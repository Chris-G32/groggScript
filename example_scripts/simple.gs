/*var x=14.25;
var x2=8.25;
var res=x+x2;
fn add_or_subtract(val:float,diff:float)->float{
print(toString(val)+","+toString(diff));
if(val<diff){
    return add_or_subtract(diff-val,val);
}
if(val==diff){
    return 1.0;
}
if(val>diff){
    return add_or_subtract(val,val+diff);
}
}
add_or_subtract(x,x2);
print(res);*/

fn fib(number:int)->int{
if number < 2{return number;}
var prev1=1;
var prev2=0;
var current=0;
for(var a=1;a<number;a++){
current=prev1+prev2;
prev2=prev1;
prev1=current;

}
return current;
}

for(var i=0;i<10000;i++){
var res=fib(i);
var printme="Fib ("+toString(i)+") = "+toString(res);
print(printme);
}
