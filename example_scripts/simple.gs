/*var count=0;
fn poopypants(pants:string)->string{
count = count + 1;
print("clean"+pants+toString(count));
poopypants(pants);
}
var myVal=1+2;
print(toString(myVal));
print("------");
print("myVal");
poopypants(toString(myVal));
poopypants(toString(myVal));
poopypants("pants");*/

fn wrap(val:string,wrap:string)->string{
    return (wrap+val) + wrap;
}
var a= wrap(wrap("--","|"),"--");
print(a);
/*
print("I can do addition and subtraction currentyl:)");
var myDecimal=3.14;
var subtracted=myDecimal-0.112122;
print("decimal"+toString(subtracted));
var myInt=21;
// This should be zero right??
print("int");


The following script is malware :)



*/