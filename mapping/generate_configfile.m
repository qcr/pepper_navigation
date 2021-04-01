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
%  * Generate configurtion file for topological garph
%  * 
%  */



clc;
close all;
clear all;
%% 
% add line matching lib path
addpath('./linematching');

%%
% base filder where topo grapg lies
bf = '/home/suman/soft/pepper_navigation/data/tmap/'
fileID = fopen([bf,'conf.txt'],'w');

%%
hd1 ="Nodes"
nodesname =[
"PepperHome";
"Harvey";
"Rob1";
"Peter";
"ROb2";
"Cartman";
"Belinda";
"Manipulation";
"Robotics"
];

nodes = 0:length(nodesname)-1;
nodesname = [nodesname, nodes']
fprintf(fileID,'%s\n',hd1);
fprintf(fileID,'%s %2s\n',nodesname');

%%
hd2 = "Edges"
fprintf(fileID,'%s\n',hd2);
edges = [
"Edge_01" "0"  "1"
"Edge_12" "1"  "2"
"Edge_23" "2"  "3" 
"Edge_34" "3"  "4"
"Edge_45" "4"  "5"
"Edge_56" "5"  "6" 
"Edge_60" "6"  "0" 
"Edge_10" "1"  "0"
"Edge_21" "2"  "1"  
"Edge_32" "3"  "2"  
"Edge_43" "4"  "3"  
"Edge_54" "5"  "4"  
"Edge_65" "6"  "5"  
"Edge_06" "0"  "6" 
];
fprintf(fileID,'%s %s %s\n',edges');
hd3 = "Branches"
branch = [
"Branch_17c" "1"  "7"  "18"
"Branch_75c" "7"  "5"  "4"
"Branch_28c" "2"  "8"  "11"
"Branch_84c" "8"  "4"  "3"
"Branch_57c" "5"  "7"  "9"
"Branch_71c" "7"  "1"  "3"
"Branch_48c" "4"  "8"  "17"
"Branch_82c" "8"  "2"  "4"
"Branch_17a" "1"  "7"  "10"
"Branch_75a" "7"  "5"  "3"
"Branch_28a" "2"  "8"  "15"
"Branch_84a" "8"  "4"  "1"
"Branch_57a" "5"  "7"  "21"
"Branch_71a" "7"  "1"  "0"
"Branch_48a" "4"  "8"  "11"
"Branch_82a" "8"  "2"  "4"

 ];
fprintf(fileID,'%s \n',hd3);


edgelist = [];


for i=1:length(branch)
    
    bb=sscanf(branch(i,1),'%c');
    d = bb(end);
    e = str2double(bb(end-1));
    s =  str2double(bb(end-2));
    
    if(s<7 && d=='c')
        fold = ['Edge_',num2str(s-1),num2str(s)];
         sf = fold;
         df = bb;
         dr = 's';
    elseif(s<7 && d=='a')
        fold = ['Edge_',num2str(s+1),num2str(s)];
         dr = 's';
           sf = fold;
           df = bb;
    elseif (e<7 && d=='a')
        fold = ['Edge_',num2str(e),num2str(e-1)];
         sf = bb;
         df = fold;
          dr = 'e';
    elseif (e<7 && d=='c')
        fold = ['Edge_',num2str(e),num2str(e+1)];
         dr = 'e';
         sf = bb;
         df = fold;
    end
   
    
    bb = [sf,' ',df,' ' , dr]
   
    
    [imlist kimlists] = getImages([bf sf]);
    [imlist kimlistd] = getImages([bf df]);
    
    linect=[];
    
    if(dr == 's')
        kim = kimlistd{1};
        kl = kimlists;
         
    elseif(dr == 'e')
        kim = kimlists{end};
        kl = kimlistd;
         
    end
    
     L = length(kl);
         for j=1:L
            kim2 = kl{j};
            status = matchLines(kim,kim2,'matched.lines')
            matchindex  = load('matched.lines');
            linect= [linect length(matchindex)];
         end
  
         [M I]= max(linect);
         flagx = -1;
         if(I == 1) 
             flagx = 0;
         elseif ((I == L) || (linect(I-1) > linect(I+1))) 
             flagx = 1;
         elseif (linect(I-1) <=linect(I+1))
             flagx = 0;
         end

        if(flagx==1) 
            indd = I-1;
        elseif(flagx==0) 
            indd = I;
        end
        
        if(dr=='e')
             indd = indd+1;
         end
   
         matches(i) = indd-1;
       
         
    
end


branch(:,end)=matches';

fprintf(fileID,'%s %s %s %s\n',branch');
  system('rm matched.lines');   


