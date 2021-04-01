%% By suman raj bista
% function to get name of imagesfiles from folder

function [ListOfImageNames TextFileList] = getImages(folder)
% 
global fodseq;
 if nargin<1
  ImageFolder = cd;   
  folder = uigetdir(ImageFolder,'Select folder');
 end
ImageFolder = folder;
fodseq = upper(folder(end-2:end));
ListOfImageNames = {};
TextFileList = {};   
    ImageFiles = dir([ImageFolder '/*.*']);
   for Index = 1:length(ImageFiles)
        baseFileName = ImageFiles(Index).name;
        [folder, name, extension] = fileparts(baseFileName);
        extension = upper(extension);
        switch lower(extension)
            case {'.png', '.bmp', '.jpg', '.tif','.jpeg','.pgm','.ppm'}
            % Allow only PNG, TIF, JPG, or BMP images
            
            basefilename = ['/' ImageFiles(Index).name];
            ListOfImageNames = [ListOfImageNames; strcat(ImageFolder,basefilename)];
            
            case{'.txt'}
                 basefilename = ['/' ImageFiles(Index).name];
                 TextFileList = [TextFileList; strcat(ImageFolder,basefilename)];
            
         
            otherwise
        end
   end
 
    
