%% 
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
% * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
% * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
% * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
% * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
% * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
% * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
% * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
% * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
% * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
% * POSSIBILITY OF SUCH DAMAGE.
% *
% /*
%  * Author: Suman Raj Bista
%  * @Inria Rennes Breatgne Atlantique
%  * Reference images selection Refer our paper
% S. R. Bista, P. R. Giordano and F. Chaumette, "Appearance-Based Indoor Navigation by IBVS Using Line Segments," 
% in IEEE Robotics and Automation Letters, vol. 1, no. 1, pp. 423-430, Jan. 2016. doi: 10.1109/LRA.2016.252190
%  * 
%  */


clear all;
close all;                    
clc;
%%
% set path for linematching and trifocal tensor estimation
addpath('./tftLine');
addpath('./linematching');
%%

% folder that contains all the images in png
% path of basefolder. 
%%
 bf = '../roboroom';
 
 %% threshold .. no of miminum match lines
 thres = 20;
 %%
 % image seq is inside imgs folder 
 [imList]=getImages([bf '/imgs_acquired']);
 
 % key/reference images will be stored in kfls folder
 KFD = [bf '/ref_imgs'];

 
%%
mp = [];
matchedlines = [];
indxmatch=[];
aaa=[];
lc  = [];
ct = 0;
global imSize;
% imSize = [640 480];
sfc = 0;
 ct=0;
 
 %%
for j=1:length(imList)
    %% read images
       im = imList{j}
       gim = imread(imList{j});
       if(size(gim,3)==3)
           gim = rgb2gray(gim);
       end
       imSize = [size(gim,2) size(gim,1)];
       imwrite(gim,'tmpimg.pgm');
    
     %% detect lines 
          status  = detectLines('tmpimg.pgm','edlines.out');
          lines = load('edlines.out');

       ct = ct +1;
      % ct = 1;
       if ct == 1 
           %% get ist image
           istim = j;
           gim_t = gim;
           im_t = im;
           linest= lines;
           save('lines_l.tmp', 'lines', '-ascii', '-tabs');
           system(['cp ',im,' ',KFD]);
%            save([KFD,'/kl_',im(end-7:end-3),'txt'],'lines', '-ascii', '-tabs');
            dlmwrite([KFD,'/kl_',im(end-8:end-3),'txt'],lines,'delimiter','\t','precision','%.12f');
           linesmatch =[];
           matchedlines = [];
           indxmatch=[];
           pr = 1;
           disp('New Key Image Added');

       else
%           save(, 'lines', '-ascii', '-tabs');  
            dlmwrite('lines_r.tmp',lines,'delimiter','\t','precision','%.12f');
           % match 
             status = matchLines('lines_l.tmp','lines_r.tmp','matched.lines');
              matchindex  = load('matched.lines');
              if length(matchindex)<10
%                
                  status = 1;
                  sfc = sfc+1;
                  if (sfc>2)
                      ct = 0;
                      sfc = 0;
                  end
              else
                  sfc = 0;
              end                   
%% match lines         
        if status == 0
          
           dt = lines;
           linest = load('lines_l.tmp');
           lines = load('lines_r.tmp');
            matchindex = matchindex+1;

            linesprev = linest(matchindex(:,1),:);
            linest= lines(matchindex(:,2),:);
%             save('lines_t.tmp', 'linesprev', '-ascii', '-tabs');
            dlmwrite('lines_l.tmp',linesprev,'delimiter','\t','precision','%.12f');
            
           
            if ct>2 
                try
                   matchedlines = getmatchedlinesin3v(linesmatch,matchindex,lines);
                   [TT,scorecheck,validindex] = calLineReconsError(matchedlines);
                catch 
                    ct =0;  %if 3 view line m,atching fialed. Add new ref image
                    continue;
                end
%                 
                inliers = validindex<0.1;
                
                cr = sum(inliers)/length(validindex);
                
                [cr sum(inliers) length(validindex)]

                if (length(validindex) <thres || (cr <0.5 && pr<0.5))
                       ct = 0;  %if no of inilers<thres or inliers ratio in prev and current frame <0.5 => add new key image
                end
                
                pr=cr;
                
            end
            
            linesmatch= [linesprev(:,1:4) linest(:,1:4)];
        end
        
       end
end

%% add last image of sequence as ref. image
       im = imList{j-1}
       gim = imread(imList{j-1});
       imwrite(gim,'tmpimg.pgm');
    
     % detect lines 
          status  = detectLines('tmpimg.pgm','edlines.out')
          lines = load('edlines.out');
          system(['cp ',im,' ',KFD]);
          % save([KFD,'/kl_',im(end-8:end-3),'txt'],'lines', '-ascii', '-tabs');
            dlmwrite([KFD,'/kl_',im(end-8:end-3),'txt'],lines,'delimiter','\t','precision','%.12f');
            
 system('rm *.tmp *.lines *.out *.pgm');          
