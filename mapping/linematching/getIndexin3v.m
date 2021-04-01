%  * Author: Suman Raj Bista
%  * @Inria Rennes Breatgne Atlantique
function [ linesindex ] = getIndexin3v( mli,mlii )
[aaa,bbb]=ismember(mli(:,1),mlii(:,1));
bbb(bbb==0)=[];
linesindex = [mli(aaa==1,:) mlii(bbb,2)];
end

