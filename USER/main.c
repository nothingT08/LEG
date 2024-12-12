#include "TaskInit.h"


/*
                                                    .'^":;Il!iiiiii!I;:"^'.                                                     
                                           .`:!+])\fxnuuuuunnnxxxxnnnuuuunxj/)[_i;^.                                            
                                     .^l+{/xvcur/)[_<!I,"^`''.....''`^",;!>_])/jnvvnf)-!".                                      
                                 `I-|xccn/}+l"'.                                '"I<[\xvcn/]!^                                  
                             'l]fczn|?!^.                                              ^I_)xccj{i^                              
                          "+\vXn)<,.                           .                           .">{rXzf-:.                          
                       "_fXzt-:.       .`,Ii~-[1(||||((1}?+>I^..                                ,~\cXr]:                        
                    '>/XXt+"      .`;<])\fxvXUCQLYvj|}_<!I:"^'..                                   `>|cYj+^                     
                  ,[vUx?"       'I+[)(\tjjjt\{?<;^'                                                   ^~fYz);                   
                ;(XY\!.       .;_][}1(|}<I"'                                                             ;1zU/!                 
              ;|YX1:         .l_-?[}1[:                                                                    ^]cJf!               
            "1YY1,           "+_-?[}1,                          ..........                                   ^]zJ\;             
          ._cC/;             :__-?[}1-I^.                     .`:li><~++_+++<>i!l;:,"^''.                      ")UX['           
         ,/Cv~               '>--?[}1(\\)}+!,'                    ....''`^,:;l!>~+__++~<>!I:"`.                  lrCxl          
        <cC\"                 '!_][{1(|/frnuuxf(]<I"'.                          .`";!<+~~~~<<<>!;`.               '}UY?.        
      .]UY?.                    ^l_})(|/fjrnvXJLLJYcx/)[_>l:"`'..                   .`;<~~<<<<<<~>'                 <cJ('       
     .)Jz>                        .">[|/fjrnvzYJJUYXzzcvunxjf\(1}]?_+<<i!!llllll!!ii>><~~~~<<<>!;^.                  ;nCt^      
    .)Jz!                            ;\/tjxnvXJLLCUYXcvunxrjjft/\||())1{}[]?-____+_++++~~<il:"'.                      ,xCf^ .   
    }JXi .                           +//tjn):!+]1(\/tffjjjjjjff/\||()1{}}[]?-_++~<>!l;:"`'.                            ,uJ/'.   
   ~UU+                              i\tfjxx(-!^      .'^",;l>~_?][}{{{}+>!;,^`'..                                      lzJ{    
  ,cJ)                                ;]/rxucYJYu/[<;^.           .'`^,;l!><<il:^'.                                      -UYi   
. |Jv^                                  "+(xzXYCCCUYcnf([+iI,^'.           .^;i~+~<!;".                                  .fJx'  
 IXJ?                                     .,+vUJJUYXzzcvunxrf/(1}?_~>ilI;:::::;!++__+~"                                   iYU+  
 1Jc"                                       IcUJJUYznxxxxxxrrjft/\\|())1{}[]?-_~>!I,^'.                                   .jJj. 
'nJ\                                      . {UUJJUU|^.'^:I!><~+__-___+~<>!l;,"`'.                                          ?JX: 
lXJ_                                      . :fJLCJYzf{+I^            .^",,"^.                                              lYU+ 
+YY!                                         .>|cUUYzzcur\{?<!;,^`..   .'^:l>>l"                                           "cJ} 
-UYI                                            'l]jzcunnnnxrjf/|){[?_~<!lI!>+~:                                           ^vJ) 
-UYI                                               .(Xvnnt}}11)))))){{}]-~>I,`.                                            ^vJ1 
>YU>                                                ^\cvnr};.  .''```''.                                                   ,zJ] 
:zJ]                                                 .!}/rnx|?!,'                                                          >YU> 
.jJf.                                                   .,i_})(){?+!^.           .'^",,"`.                                 1Jc" 
 ?JX;          .^:!<-}(/jnvzXXzv\"                           ..'^^""'    'l~](jvYC0ZmwwmOJx-'                             `uJ| .
 ^vJ(     . .~fengbaofengbaofengd!                                     ^(Lqdbdw0JcxrjfjxzObk\                             -UYl  
  ?JXl   . "ndbwQYuf|{]+<!I:"?pq[                                     _OkbqZfl^';]\?.    <pO+                            "vJ|.  
  'jJr`   .nbZc|-"          ;CO+                                   . .[|?<;`  +cmJ};;!~])/["                           ..|Jv,   
   ;cJ(. ..l!"'.           ]Zc:    ^~)fnx~    ''   .l-,     `i1xXu,     ~[{)/UqQc|1){]~l"      `l]/nc/`      ">-?l      ?UY<    
    >XJ}             '^``lvw("   !jCv{(dq?  +cQ/'>/Jpk|  `-nOwLx{]\j" .'1vwkb0Yt\)[+I'   .  "?uOpmJLkr'   "}XwqOd0~    ~YU?     
   . <XJ}    ..'_\rcJQ00OpkpY?.,npQ]+\v(: ;vpU[{XY0kJ?''{QdU{:.;fmkv. . IvqY_'`,Ii_}x0v~. !rmwY(>>fwm]   }0bJ(~Xpk\   ~XU[      
    . >zJ)' .. lvzcurzpbJ/\|l .vkp/?~:  l]Qku1Yu-jZ{'^+nbO< .<nLmdr">;.{wkj        :\dkY.-pq)" ;|Lkc,  :/bm+  )bdXIl.?YY?       
       InCf,       ^)QJ]'     .Ykp\l!_\n(<Okqc>^ckZ\rr-|bL{jJU{/pL|\["ldbpOr1[})fcQpdL/: ?kL>{cz)Jbu]|j1fkX",<XqQn(_)Jz<        
        ^\Jc~. ">]fQX-'        !cOwOJn[,  ~j+  ;C0Y/i. .]vn)!,\qY{l.  `)JZqbbbpwOUr{i`   .{Q0v_'lqpmX{: '(CQLJ/l''lrCj;         
       . ._cUfuZp0f!         ..  ^,"'           '^    .   .>jZJ>        .,Iii!I,`          '`   '!>,      ':,.  `{YY}'          
           ,YddC_'                                    . ']Cbq\`                                               .+uCfI            
          . ">fYn_`                                    :Xkmj;                                               .<rJn~.             
              .>jJu?^                                  +u}:  ..',!+}|t/(1{]-+<!!l;,^.                     '+rJn_'               
                .itUz1I                                      ..."l~](/()1[]?+>i!I:"`                    ,[vUr+'                 
                   ;1cYj_"               _>  ^i     '><<<:     I,     I!i>"    '!!+ii`               ^</YX|!.                   
                     `~/XXt+"            n}}"l/    't!`':<.    |_    "f+!~+    .^"u:"'            ^<|cYj-"                      
                        "+tzXj]I'       .x';{\/    'jI'':+.    |+    :_Il_\.     .n.          .;-/zXj?:                         
                           ^<)nXv/?l`    i. .+i     ^~~<~:     i:    '~~<+>.     .-.      ';_|nXv\+,                            
                              .:+|nzcj)+I`.                                         .^;~{fvzu\?I'                               
                                  .">[/nccnt1->;"'.                          '^:!_}\xvcut}<:'                                   
                                       .^I~[|jnvvnxf\)}]_~>i!lllll!i>~_?}1|trnuvnr\{+!"'                                        
                                              '^;!+]{|/jxnvvvcczcccvvnxjt|1]_i;"'                                               
*/




int main(void)
{ 
	
	
	SYSTEM_Init();
	
	
}



















