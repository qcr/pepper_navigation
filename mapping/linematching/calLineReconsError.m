function [T,scorecheck,validindex] = calLineReconsError(matchedlines)

global imSize;

[matchedlines,H] = normalizePoints(matchedlines,imSize);
dxy = matchedlines(:,2:2:end,:)- matchedlines(:,1:2:end,:);
lines2d = [dxy(2,:,:);-dxy(1,:,:);matchedlines(2,1:2:end,:).*dxy(1,:,:) - matchedlines(1,1:2:end,:).*dxy(2,:,:)];


for j = 1:size(lines2d,2)
      for i= 1:3
          lines2d(:,j,i) = lines2d(:,j,i)/norm(lines2d(:,j,i),2);
      end
end

[Tx, e2, e3,inliers] = ransacfitTrifocalTensor_lines(lines2d,0.001,0);

 T = Tx;
% P2 = getPfromT(T,e2,e3);
% for i = 1:3
%     den = nthroot(det(P2(:,1:3,i)),3);
% %     P1(:,:,i) = P1(:,:,i)/den;
%     P2(:,:,i) = P2(:,:,i)/den;
% end
 for j = 1:size(lines2d,2)
%       for i= 1:3
%           lines2d(:,j,i) = lines2d(:,j,i)/norm(lines2d(:,j,i),2);
%       end
      l1 = lines2d(:,j,1);
      l2 = lines2d(:,j,2);
      l3 = lines2d(:,j,3);
      l1cal = [l2'*T(:,:,1)*l3;l2'*T(:,:,2)*l3;l2'*T(:,:,3)*l3];
         sm  = cross(l1,l1cal);
%           scorecheck(:,j) = norm(sm);
           
           
       l1cal = l1cal/norm(l1cal,2);
       
        sm  = cross(l1cal,l1);
      
        validindex(j)=norm(sm);
%         diff2(:,j)=abs(l1)-abs(l1cal);
  end
% L3 = lineTx( P2,lines2d );
%  L3 = inliers;
%  F21 = skew(e2)*[Tx(:,:,1)*e3 Tx(:,:,2)*e3 Tx(:,:,3)*e3];
%  E21 = computeEfromF(F21,eye(3))
%  
%  F31 = skew(e3)*[Tx(:,:,1)'*e2 Tx(:,:,2)'*e2 Tx(:,:,3)'*e2];
%  E31 = computeEfromF(F31,eye(3))
%  
% %  Tx = tt;
%  F21 = skew(e2)*[Tx(:,:,1)*e3 Tx(:,:,2)*e3 Tx(:,:,3)*e3];
%  E21 = computeEfromF(F21,eye(3))
%  
%  F31 = skew(e3)*[Tx(:,:,1)'*e2 Tx(:,:,2)'*e2 Tx(:,:,3)'*e2];
%  E31 = computeEfromF(F31,eye(3))
scorecheck = inliers;

%    
%  for j = 1:size(lines2d,2)
%       l1 = lines2d(:,j,1);
%       l2 = lines2d(:,j,2);
%       l3 = lines2d(:,j,3);
%       l1cal = [l2'*T(:,:,1)*l3;l2'*T(:,:,2)*l3;l2'*T(:,:,3)*l3];
%      
%       diff(:,j)=l1-l1cal;
%  end
%   
%  keyboard;
 
 

%   for j = 1:size(lines2d,2)
%       for i= 1:3
%           lines2d(:,j,i) = lines2d(:,j,i)/norm(lines2d(:,j,i),2);
%       end
%   end
% P1
%  P2
%  [errrs,errrrrsss] = LineReconst(P2,lines2d) ;    

      
        
for ii = 1:3
    Y(:,:,ii) = inv(H) * T(:,:,ii) * (inv(H))';
end
    
for ii = 1:3
    T(:,:,ii) = H(1,ii)*Y(:,:,1) + H(2,ii)*Y(:,:,2) + H(3,ii)*Y(:,:,3);
end

end

