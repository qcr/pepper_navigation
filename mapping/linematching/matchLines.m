%  * Author: Suman Raj Bista
%  * @Inria Rennes Breatgne Atlantique
function [ status,result ] = matchLines(l1,l2,ml)

[status, result] = system(['./linematching/matchlines ',l1,' ',l2,' ',ml]);


end
