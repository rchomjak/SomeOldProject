
% Author: knajt, knight(at)wilbury.sk 
% Date:  2011 
% example: lagrange([-1,1,2,3],[-6,-2,-3,2])

function [y]=lagrange(X,Y)                  

max_size_var=max(size(X));                   
n=max_size_var;                             
                                          
solution=0;                                
x=sym('x');                               

if max(size(X))~=max(size(Y))              
   display('Chyba: vektory X a Y musi mit stejny rozmer'); 
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
expand(solution);                            
y=expand(solution);                        
  

   Z=X(1):1/1000:X(n);                    
graf=sym2poly(y);                          
graf=polyval(graf,Z);                      

                                    
figure                                      
hold on                                     
grid on                                     
xlabel('Souradnice X');                     
ylabel('Souradnice Y');                     

  
plot(Z,graf,'Color',[0 0 1]);             
title('Lagrangeuv interpolacni polynom');  

plot(X,Y,'+');                             
  
end;
