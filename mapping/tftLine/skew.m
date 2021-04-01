function R = skew(t)
%#codegen
R = [0 -t(3) t(2);t(3) 0 -t(1);-t(2) t(1) 0];