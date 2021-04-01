function [lines2d,rTh] = getLinesFromEndPoints(linepoints2D)
dxy = linepoints2D(:,2:2:end,:)- linepoints2D(:,1:2:end,:);
 lines2d = [dxy(2,:,:);-dxy(1,:,:);linepoints2D(2,1:2:end,:).*dxy(1,:,:) - linepoints2D(1,1:2:end,:).*dxy(2,:,:)];
 
  d = sqrt(dxy(1,:,:).^2 +  dxy(2,:,:).^2);
  r = (-linepoints2D(2,1:2:end,:).*dxy(1,:,:) + linepoints2D(1,1:2:end,:).*dxy(2,:,:))./d;
  s = sign(r);
 rTh = [atan2d(-dxy(1,:,:).*s,dxy(2,:,:).*s);r.*s];