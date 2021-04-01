function  [points2d, T ] = normalizePoints(points2d,imSize)

if size(points2d,1) ==2
    points2d(end+1,:,:) = 1;
end

NmatInv =  [mean(imSize) 0 imSize(2)/2;0 mean(imSize) imSize(1)/2;0 0 1];
% NmatInv = [200     0   128
%              0   200   128
%              0     0     1];
Nmat   = inv(NmatInv);    

for i = 1:size(points2d,3)
    points2d(:,:,i) = Nmat*points2d(:,:,i);                                                    
end

T = Nmat;