fn countRecurse(count:int)->int{
    if (count < 1) {
        return 1;
    }
    countRecurse(count-1);
    print(toString(count));
    return 1;
}
fn repeat(str:string,count:int)->string{
    if (count < 1) {
    return "";
    }
    if(count < 2){
        return str;
     }
    return str + repeat(str,count-1);
}
fn fib(num:int)->int{
if num <2{return num;}
return fib(num-1)+fib(num-2);
}
print("\"countRecurse\"\n");
countRecurse(2);

print("fib");
print(toString(fib(20))+"\n");

print(repeat("_",2000));