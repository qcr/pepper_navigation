% RANSACFITFUNDMATRIX - fits fundamental matrix using RANSAC
%
% Usage:   [F, inliers] = ransacfitfundmatrix(x1, x2, t)
%
% Arguments:
%          x1  - 2xN or 3xN set of homogeneous points.  If the data is
%                2xN it is assumed the homogeneous scale factor is 1.
%          x2  - 2xN or 3xN set of homogeneous points such that x1<->x2.
%          t   - The distance threshold between data point and the model
%                used to decide whether a point is an inlier or not. 
%                Note that point coordinates are normalised to that their
%                mean distance from the origin is sqrt(2).  The value of
%                t should be set relative to this, say in the range 
%                0.001 - 0.01  
%
% Note that it is assumed that the matching of x1 and x2 are putative and it
% is expected that a percentage of matches will be wrong.
%
% Returns:
%          F       - The 3x3 fundamental matrix such that x2'Fx1 = 0.
%          inliers - An array of indices of the elements of x1, x2 that were
%                    the inliers for the best model.
%
% See Also: RANSAC, FUNDMATRIX

% Copyright (c) 2004-2005 Peter Kovesi
% School of Computer Science & Software Engineering
% The University of Western Australia
% http://www.csse.uwa.edu.au/
% 
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, subject to the following conditions:
% 
% The above copyright notice and this permission notice shall be included in 
% all copies or substantial portions of the Software.
%
% The Software is provided "as is", without warranty of any kind.

% February 2004  Original version
% August   2005  Distance error function changed to match changes in RANSAC

function [T,e2,e3,inliers] = ransacfitTrifocalTensor_lines(lines2d, t, feedback)
%#codegen
assert(isa(lines2d, 'double'));
x1 = lines2d(:,:,1);
x2 = lines2d(:,:,2);
x3 = lines2d(:,:,3);

%     if ~all(size(x1)==size(x2))
%         error('Data sets x1 and x2 must have the same dimension');
%     end
%     
%     if nargin == 3
% 	feedback = 0;
%     end
%     
%     [rows,npts] = size(x1);
%     if rows~=2 & rows~=3
%         error('x1 and x2 must have 2 or 3 rows');
%     end
%     
%     if rows == 2    % Pad data with homogeneous scale factor of 1
%         x1 = [x1; ones(1,npts)];
%         x2 = [x2; ones(1,npts)];    
%         x3 = [x3; ones(1,npts)];    
%     end
    
    % Normalise each set of points so that the origin is at centroid and
    % mean distance from origin is sqrt(2).  normalise2dpts also ensures the
    % scale parameter is 1.  Note that 'fundmatrix' will also call
    % 'normalise2dpts' but the code in 'ransac' that calls the distance
    % function will not - so it is best that we normalise beforehand.
%     [x1, T1] = normalise2dpts(x1);
%     [x2, T2] = normalise2dpts(x2);
%     [x3, T3] = normalise2dpts(x3);

    s = 13;  % Number of points needed to fit a fundamental matrix. Note that
            % only 7 are needed but the function 'fundmatrix' only
            % implements the 8-point solution.
    
    fittingfn = @trifocaltensor_line;
    distfn    = @funddist;
    degenfn   = @isdegenerate;
    % x1 and x2 are 'stacked' to create a 6xN array for ransac
    [T,inliers] = ransac([x1; x2; x3], fittingfn, distfn, degenfn, s, t, feedback);

    % Now do a final least squares fit on the data points considered to
    % be inliers.
   if length(inliers)>12
        TT = trifocaltensor_line(x1(:,inliers), x2(:,inliers),x3(:,inliers));
        T = TT.T;
        e2 = TT.e2;
        e3 = TT.e3;
   else
       [e2,e3] = e_from_T(T);
   end
       
%       for ii = 1:3
%         Y(:,:,ii) = inv(T2) * T(:,:,ii) * (inv(T3))';
%       end
%     
%     for ii = 1:3,
%         T_denorm(:,:,ii) =   T1(1,ii)*Y(:,:,1) ...
%                            + T1(2,ii)*Y(:,:,2) ...
%                            + T1(3,ii)*Y(:,:,3);
%     end
%     
%     
%     % Denormalise
%     T = T_denorm;
   
    
    
end
    
%--------------------------------------------------------------------------
% Function to evaluate the first order approximation of the geometric error
% (Sampson distance) of the fit of a fundamental matrix with respect to a
% set of matched points as needed by RANSAC.  See: Hartley and Zisserman,
% 'Multiple View Geometry in Computer Vision', page 270.
%
% Note that this code allows for F being a cell array of fundamental matrices of
% which we have to pick the best one. (A 7 point solution can return up to 3
% solutions)

function [bestInliers,T] = funddist(T, x, t)
    
    x1 = x(1:3,:);    % Extract x1 and x2 from x
    x2 = x(4:6,:);
    x3 = x(7:9,:);

    e2 = T.e2;
    e3 = T.e3;
    T = T.T;
    To = T;
     nlines = size(x1,2);
     
 bb= repmat(x2',[1,3]);
 aa=[repmat(x3(1,:)',1,3),repmat(x3(2,:)',1,3),repmat(x3(3,:)',1,3)];
 ab = aa.*bb;
 
  A = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*ab  repmat((x1(2,:))',1,9).*ab; ...
      repmat((x1(3,:))',1,9).*ab zeros(nlines,9)  -repmat((x1(1,:))',1,9).*ab];  
    
 Ja1 = [zeros(nlines,9) zeros(nlines,9) zeros(nlines,9); ...
        zeros(nlines,9) zeros(nlines,9) ab]; 
 Jb1 = [zeros(nlines,9) zeros(nlines,9) ab;
        zeros(nlines,9) zeros(nlines,9) zeros(nlines,9)];
 Jc1 = [zeros(nlines,9) -ab zeros(nlines,9);
        ab zeros(nlines,9) zeros(nlines,9)];
    
dl2a =  repmat([1 0 0],nlines,3).*aa;

Ja2 = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*dl2a repmat((x1(2,:))',1,9).*dl2a ;
       repmat((x1(3,:))',1,9).*dl2a zeros(nlines,9) repmat((x1(1,:))',1,9).*dl2a ];
   
dl2a =  repmat([0 1 0],nlines,3).*aa;

Jb2 = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*dl2a repmat((x1(2,:))',1,9).*dl2a ;
       repmat((x1(3,:))',1,9).*dl2a zeros(nlines,9) repmat((x1(1,:))',1,9).*dl2a ];
   
dl2a =  repmat([0 0 1],nlines,3).*aa;

Jc2 = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*dl2a repmat((x1(2,:))',1,9).*dl2a ;
       repmat((x1(3,:))',1,9).*dl2a zeros(nlines,9) repmat((x1(1,:))',1,9).*dl2a ];
   
dl2a = [ones(nlines,3) zeros(nlines,3) zeros(nlines,3)].*bb;

Ja3 = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*dl2a repmat((x1(2,:))',1,9).*dl2a ;
       repmat((x1(3,:))',1,9).*dl2a zeros(nlines,9) repmat((x1(1,:))',1,9).*dl2a ];
   
dl2a = [zeros(nlines,3) ones(nlines,3) zeros(nlines,3)].*bb;

Jb3 = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*dl2a repmat((x1(2,:))',1,9).*dl2a ;
       repmat((x1(3,:))',1,9).*dl2a zeros(nlines,9) repmat((x1(1,:))',1,9).*dl2a ];

dl2a = [zeros(nlines,3) zeros(nlines,3) ones(nlines,3)].*bb;

Jc3 = [zeros(nlines,9) -repmat((x1(3,:))',1,9).*dl2a repmat((x1(2,:))',1,9).*dl2a ;
       repmat((x1(3,:))',1,9).*dl2a zeros(nlines,9) repmat((x1(1,:))',1,9).*dl2a ];

   TT = T(:);
   bigE = A*TT;
 
    
    JM = [Ja1*TT Jb1*TT Jc1*TT Ja2*TT Jb2*TT Jc2*TT Ja3*TT Jb3*TT Jc3*TT];
    for i = 1:nlines
        
        J = [JM(i,:);JM(nlines+i,:)];
        errs =[bigE(i,:);bigE(nlines+i,:)];
        d(i)= errs'*((J*J')\errs);        
    end


	bestInliers = find(abs(d) < t);    % Indices of inlying points
                        % Copy F directly to bestF
	T= To;
    end
	


%----------------------------------------------------------------------
% (Degenerate!) function to determine if a set of matched points will result
% in a degeneracy in the calculation of a fundamental matrix as needed by
% RANSAC.  This function assumes this cannot happen...
     
function r = isdegenerate(x)
    r = 0;  
end
    
