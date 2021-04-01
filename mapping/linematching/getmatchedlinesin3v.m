%  * Author: Suman Raj Bista
%  * @Inria Rennes Breatgne Atlantique
function matchedlines = getmatchedlinesin3v(linesmatch,matchindex,lines)

%      matchedlines = [linesmatch(matchindex(:,1),:) lines(matchindex(:,2),:)];
npts = size(matchindex,1);
matchedlines = zeros(2,2*npts,3);

matchedlines(1,1:2:2*npts,1) = linesmatch(matchindex(:,1),1)';
matchedlines(2,1:2:2*npts,1) = linesmatch(matchindex(:,1),2)';
matchedlines(1,2:2:2*npts,1) = linesmatch(matchindex(:,1),3)';
matchedlines(2,2:2:2*npts,1) = linesmatch(matchindex(:,1),4)';

matchedlines(1,1:2:2*npts,2) = linesmatch(matchindex(:,1),5)';
matchedlines(2,1:2:2*npts,2) = linesmatch(matchindex(:,1),6)';
matchedlines(1,2:2:2*npts,2) = linesmatch(matchindex(:,1),7)';
matchedlines(2,2:2:2*npts,2) = linesmatch(matchindex(:,1),8)';

matchedlines(1,1:2:2*npts,3) = lines(matchindex(:,2),1)';
matchedlines(2,1:2:2*npts,3) = lines(matchindex(:,2),2)';
matchedlines(1,2:2:2*npts,3) = lines(matchindex(:,2),3)';
matchedlines(2,2:2:2*npts,3) = lines(matchindex(:,2),4)';

