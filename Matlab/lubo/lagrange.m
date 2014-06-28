
% Author: knajt, knight(at)wilbury.sk 
% Date:  2011 
% example: lagrange([-1,1,2,3],[-6,-2,-3,2])

function [y]=lagrange(X,Y)                  

n=max(size(X));                   
                                          
solution=0;                                
x=sym('x');                               

if max(size(X))~=max(size(Y))              
   display('Chyba: vektory X a Y musia mat rovnaky rozmer.'); 
else                                        
  for i=1:n                           
      L=1;                                  
      for j=1:n                             
         if i~=j                                           
        L=L*(x-X(j))/(X(i)-X(j));           
         end
         
      end
       solution=solution+(L*Y(i));         
  end
 solution=simplify(solution);               
y=expand(solution);                        
  

Z=X(1):1/1000:X(n);                    
graf=sym2poly(y);                          
graf=polyval(graf,Z);                      

                                    
figure                                      
hold on                                     
grid on                                     
xlabel('Suradnice X.');                     
ylabel('Suradnice Y.');                     

  
plot(Z,graf,'Color',[0 0 1]);             
title('Lagrangeov interpolacny polynom');  

plot(X,Y,'+');                             
  
end;
