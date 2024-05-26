#include <iostream>

class Base {
public:
    int publicVar; // Accessible from anywhere

protected:
    int protectedVar; // Accessible within the class, derived classes, and friends

private:
    int privateVar; // Accessible only within the class or friends
};

class Derived : public Base {
    void accessMembers() {
        publicVar = 1; // Allowed: public members are accessible in derived class
        protectedVar = 2; // Allowed: protected members are accessible in derived class
        // privateVar = 3; // Error: private members are NOT accessible in derived class
    }
};

int main() {
    Base obj;
    obj.publicVar = 1; // Allowed: public members are accessible outside the class
    // obj.protectedVar = 2; // Error: protected members are NOT accessible outside the class
    // obj.privateVar = 3; // Error: private members are NOT accessible outside the class
}
