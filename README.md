# RSF
RSF (Really Stupid Format) is a scripting language made in C++
# Sample Code
<img src="imgs/sample.pmg">
```
void main()
{
    var HelloWorld = "Hello, World!"
    print((HelloWorld) & (endl))
    getcin(cin)
    if (cin) == "looptest" -> looptest
    if (cin) == "returntest" -> returntest
}

void returntest()
{
    returna()
    print("returna returned: " & (return) & (endl))
}

void returna()
{
    <- Hello
}

void looptest()
{
    print("Hello, World!" & (endl))
    wait(700)
    looptest()
}
```