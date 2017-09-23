void function_b2();

bool function_foo() {
    function_b2();
    return true;
}

bool function_bar() {
    return function_foo();
}

void function_a() {
    function_bar();
    function_foo();
}

void function_b() {
    function_bar();
    function_foo();
}

void function_b2() {
    function_bar();
    //function_foo();
    
    function_a();
    function_b();
}

int main(void) {
    function_foo();
    function_bar();
    //function_a();
    function_b();
    function_b2();
    
    return 42;
}
