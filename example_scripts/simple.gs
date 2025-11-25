var x=14.25;
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
print(res);