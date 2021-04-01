%  * Author: Suman Raj Bista
%  * @Inria Rennes Breatgne Atlantique

function [ status ] = detectLines(im, outfile )

[status, result] = system(['./linematching/detectlinesED ',im,' ',outfile]);

end

