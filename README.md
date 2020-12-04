# cs_checker

Compilation:
gcc -Werror -Wall -Wextra -stdc=99 -pedantic checker.c -o checker

Extra: Rajouter une fonction en shell permettant de le lancer de n'importe ou
```
RED="\033[1;31m"                                                                
GREEN="\033[1;32m"                                                              
NOCOLOR="\033[0m"                                                               
                                                                                 
function check () {                                                             
    cp ~/Documents/projet_perso/cs_checker/checker .                            
    ./checker "$1" 1> tmp                                                       
    fail="$?"                                                                   
    if [ ${fail} -eq 1 ]; then                                                  
        echo "${RED}-------------------------------${NOCOLOR}"                  
        echo "${RED}CODING-STYLE CHECKER   $1${NOCOLOR}"                        
        echo "${RED}-------------------------------${NOCOLOR}"                  
    else                                                                        
        echo "${GREEN}-------------------------------${NOCOLOR}"                
        echo "${GREEN}CODING-STYLE CHECKER   $1${NOCOLOR}"                      
        echo "${GREEN}-------------------------------${NOCOLOR}"                
    fi                                                                          
    cat tmp                                                                     
    if [ ${fail} -eq 1 ]; then                                                  
        echo "${RED}FAIL${NOCOLOR}\n"                                           
    else                                                                        
        echo "${GREEN}PASS${NOCOLOR}\n"                                         
        clang-format -i "$1"                                                    
    fi                                                                          
    rm checker tmp                                                              
 }
```
