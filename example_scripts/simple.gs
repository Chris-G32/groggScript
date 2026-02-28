/*
var i = "sa";
var a = "1";
var b= a < i;
print(b);
print(1);
print(1.234);
print("hay");
*/

var synonyms=[["poopy","pants","poopy"],["john","pants"],["beast games season 1"]];
var synonymsExplicit:string[][]=[["hi","howdy","heyy"],["happy","ecstatic"],["door"]];
print(synonyms);
for ( var i=0;i<3; i++){
    print(atArr(synonyms,i));

}
print(atArr(atArr(synonyms,0),0)==atArr(atArr(synonymsExplicit,0),0));