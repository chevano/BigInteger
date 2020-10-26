#include <iostream>
#include <string>
#include <cstring> // Needed Depending on your compiler(strcpy)

using namespace std;

class BigInt {
   private:
    int *num;
    int size;

   public:
    BigInt();
    BigInt(string);
    BigInt(int);
    BigInt(const BigInt &);
    ~BigInt();

    int isLarger(BigInt, BigInt);
    BigInt reverse(const BigInt &);
    void computeSum(BigInt&, BigInt&, BigInt&);
    void computeDiff(BigInt&, BigInt&, BigInt&, int&);
    void computeMul(BigInt&, BigInt&, BigInt&);
    void setIsSize(const int&, const int&, int&, bool&, int&);
    void setSign(const int&, const int&, const int&, const int&, BigInt&);
    void removeLeadingZeros(BigInt &);

    BigInt operator+(const BigInt &);
    BigInt operator-(const BigInt &);
    BigInt operator*(const BigInt &);
    BigInt& operator=(const BigInt &);
    friend ostream& operator<<(ostream& os, const BigInt &);
};

int main()
{
    BigInt b1("123456789123");
    BigInt b2("1234567891234");

    BigInt b3 = b1 + b2;
    BigInt b4 = b1 - b2;
    BigInt b5 = b2 - b1;
    BigInt b6 = b1 * b2;

    cout << b1 << " + " << b2 << " = " << b3 << endl;
    cout << b1 << " - " << b2 << " = " << b4 << endl;
    cout << b2 << " - " << b1 << " = " << b5 << endl;
    cout << b1 << " * " << b2 << " = " << b6 << endl;
    
    return 0; 
}
/********************************
*       Default Constructor
*********************************/
BigInt::BigInt() {
    size = 1000;
    num = new int[size];
}

/********************************
*       Generic Constructor
*********************************/
BigInt::BigInt(int size) {
    this->num = new int[size];
    this->size = size;

    for(int i = 0; i < size; i++)
        num[i] = 0;
}

/********************************
*       Generic Constructor
*********************************/
BigInt::BigInt(string str) {
    this->size = str.length();
    this->num = new int[size];
    char *strCH = new char[size + 1];
    
    strcpy(strCH, str.c_str());

    for(int i = 0; i < size; i++)
        num[i] = strCH[i] - '0';
}

/********************************
*       Copy Constructor
*********************************/
BigInt::BigInt(const BigInt & oldObj) {
    num = new int[oldObj.size];
    size = oldObj.size;

    for(int i = 0; i < oldObj.size; i++)
        num[i] = oldObj.num[i];
}

/********************************
*        Destructor
*********************************/
BigInt::~BigInt() {
    delete [] num;
}

/**************************************************
*       returns -1 if lhs < rhs
*       returns 0 if lhs = rhs
*       returns 1 if lhs > rhs
**************************************************/
int BigInt::isLarger(BigInt lhs, BigInt rhs) {
    
    int length = lhs.size;
    
    for(int i = 0; i < length; i++) {
        if(lhs.num[i] > rhs.num[i])
            return 1;
            
        else if(lhs.num[i] < rhs.num[i])
            return -1;
    }
    
    return 0;
}

/**************************************************
*               Reverses a BigInt
**************************************************/
BigInt BigInt::reverse(const BigInt &bigInt) {
    BigInt temp;
    temp.num = new int[bigInt.size];
    temp.size = bigInt.size;
    
    int index = 0;

    for(int i = bigInt.size - 1; i >= 0; i--) {
        temp.num[index++] = bigInt.num[i];
    }

    return temp;
}

/**************************************************
*       Removes all the leading zeros
*       in a BigInt Object
**************************************************/
void BigInt::removeLeadingZeros(BigInt &bigInt) {
    int index = -1;
    
    // Finds the first occurence of a non-zero element
    for(int i = 0; i < bigInt.size; i++) {
        if(bigInt.num[i] != 0) {
            index = i;
            break;
        }
    }
    // Check whether the array is filled with all zeros
    if(index == -1) {
        delete [] bigInt.num;
        bigInt.num = new int[1];
        bigInt.size = 1;
        bigInt.num[0] = 0;
    }

    // Creates a new array with a reduce size where all the leading zeros are removed
    else {
        int reduceSize = bigInt.size - index;
        int *temp = new int[reduceSize];

        for(int i = 0; i < reduceSize; i++)
            temp[i] = bigInt.num[index + i];

        delete [] bigInt.num;
        bigInt.num = new int[reduceSize];
        bigInt.size = reduceSize;

        for(int i = 0; i < reduceSize; i++)
            bigInt.num[i] = temp[i];

        delete [] temp;
    }
}

/**************************************************
*       Sets the variables sign, isSize,
*       largerSize depending on the size
*       of the lhs object and the rhs object
**************************************************/
void BigInt::setIsSize(const int& lhsSize, const int& rhsSize, int& largerSize, bool& isSize, int& sign) {
    if(lhsSize >= rhsSize) {
        sign = 1;
        isSize = true;
        largerSize = lhsSize;
    }

    else {
        sign = -1;
        isSize = false;
        largerSize = rhsSize;
    }
}

/**************************************************
*       Computes the sum of two BigInt
**************************************************/
void BigInt::computeSum(BigInt& bigInt, BigInt& lhsTemp, BigInt& rhsTemp) {
    for(int i = 0; i < bigInt.size; i++) {
        bigInt.num[i] += lhsTemp.num[i] + rhsTemp.num[i];

        if(bigInt.num[i] > 9) {
            bigInt.num[i] = bigInt.num[i] % 10;
            bigInt.num[i+1] += 1;
        }
    }  
}

/**************************************************
*       Computes the product of two BigInt
**************************************************/
void BigInt::computeMul(BigInt& bigInt, BigInt& lhsTemp, BigInt& rhsTemp) {
    for(int i = 0; i < lhsTemp.size; i++) {
        int carry = 0;
        for(int j = 0; j < rhsTemp.size; j++) {
            int x = lhsTemp.num[i] * rhsTemp.num[j] + bigInt.num[i+j] + carry;
            carry = x / 10;
            bigInt.num[i+j] = x % 10;
        }
    }
}

/**************************************************
*       Computes the difference of two BigInt
**************************************************/
void BigInt::computeDiff(BigInt& bigInt, BigInt& lhsTemp, BigInt& rhsTemp, int& largerObject) {
    for(int i = 0; i < bigInt.size; i++) {
        if(largerObject == -1) {
            if(rhsTemp.num[i] >= lhsTemp.num[i])
                bigInt.num[i] = rhsTemp.num[i] - lhsTemp.num[i];
            else {
                rhsTemp.num[i+1] -= 1;
                rhsTemp.num[i] += 10;
                bigInt.num[i] = rhsTemp.num[i] - lhsTemp.num[i];
            }
        }

        else {
            if(lhsTemp.num[i] >= rhsTemp.num[i])
                bigInt.num[i] = lhsTemp.num[i] - rhsTemp.num[i];
                
            else {
                lhsTemp.num[i+1] -= 1;
                lhsTemp.num[i] += 10;
                bigInt.num[i] = lhsTemp.num[i] - rhsTemp.num[i];
            }
        }
    }
}

void BigInt::setSign(const int& lhsSize, const int& rhsSize, const int& sign, const int& largerObject, BigInt& result) {
    if(lhsSize < rhsSize)
        result.num[0] *= sign;
    else {
        if(largerObject != 0)
            result.num[0] *= largerObject;
    }
}

/**************************************************
*       Overload the extraction operator 
*       for printing a BigInt
**************************************************/
ostream& operator<<(ostream& os, const BigInt& bigInt) {
    for(int i = 0; i < bigInt.size; i++) {
        os << bigInt.num[i];
    }

    return os;
}

/**************************************************
*       Overload the = operator 
*       for storing a BigInt into another BigInt
**************************************************/
BigInt& BigInt::operator=(const BigInt& obj) {
    // Checks whether the object on the left hand side
    // is different from the object on the right hand side
    // of the = operator
    if(this != &obj) {
        delete [] num;
        size = obj.size;
        num = new int[size];

        for(int i = 0; i < obj.size; i++)
            num[i] = obj.num[i];
    }

    return *this;
}

/**************************************************
*       Overload the + operator 
*       to perform addition 
**************************************************/
BigInt BigInt::operator+(const BigInt& rhs) { 
    int largerSize, sign, index = 0;

    // Set to true if the object on the left hand side
    // size is greater than or equal to the object on
    // the right hand side
    bool isSize; 

    BigInt lhsCopy = *this; // Object on the left hand side of the + operator
    BigInt rhsCopy = rhs;   // Object on the right hand side of the + operator

    setIsSize(size, rhs.size, largerSize, isSize, sign);

    BigInt temp(largerSize);

    int sizeDiff = abs(lhsCopy.size - rhsCopy.size);

    for(int i = sizeDiff; i < largerSize; i++) {
        if(isSize)
            temp.num[i] = rhsCopy.num[index++];
        else 
            temp.num[i] = lhsCopy.num[index++];
    }

    BigInt lhsTemp, rhsTemp;

    if(isSize) {
        lhsTemp = reverse(lhsCopy);
        rhsTemp = reverse(temp);
    }
    else {
        lhsTemp = reverse(temp);
        rhsTemp = reverse(rhsCopy);
    }

    BigInt bigInt(largerSize);
    computeSum(bigInt,lhsTemp,rhsTemp);
    BigInt result = reverse(bigInt);

    return result;
}

/**************************************************
*       Overload the - operator 
*       to perform subtraction 
**************************************************/
BigInt BigInt::operator-(const BigInt& rhs) { 

    int largerSize, largerObject, sign;

    // Set to true if the object on the left hand side
    // size is greater than or equal to the object on
    // the right hand side
    bool isSize;

    BigInt rhsCopy = rhs;   // Object on the left hand side of the - operator
    BigInt lhsCopy = *this; // Object on the right hand side of the - operator

    setIsSize(size, rhs.size, largerSize, isSize, sign);

    BigInt temp(largerSize);
    BigInt bigInt(largerSize);

    int sizeDiff = abs(lhsCopy.size - rhsCopy.size);
    int index = 0;

    for(int i = sizeDiff; i < largerSize; i++) {
        if(isSize)
            temp.num[i] = rhsCopy.num[index++];
        else
            temp.num[i] = lhsCopy.num[index++];
    }

    BigInt lhsTemp, rhsTemp;
    if(isSize) {
        largerObject = isLarger(lhsCopy,temp);
        lhsTemp = reverse(lhsCopy);
        rhsTemp = reverse(temp);
    }
        
    else {
        largerObject = isLarger(temp,rhsCopy);
        lhsTemp = reverse(temp);
        rhsTemp = reverse(rhsCopy);
    }

    computeDiff(bigInt, lhsTemp, rhsTemp, largerObject);
    BigInt result = reverse(bigInt);
    removeLeadingZeros(result);
    setSign(size, rhs.size, sign, largerObject, result);

    return result;
}

/**************************************************
*       Overload the * operator 
*       to perform multiplication 
**************************************************/
BigInt BigInt::operator*(const BigInt& rhs) {
    int largerSize, sign, index = 0;

    // Set to true if the object on the left hand side
    // size is greater than or equal to the object on
    // the right hand side
    bool isSize; 

    BigInt lhsCopy = *this; // Object on the left hand side of the + operator
    BigInt rhsCopy = rhs;   // Object on the right hand side of the + operator

    setIsSize(size, rhs.size, largerSize, isSize, sign);

    BigInt lhsTemp = reverse(lhsCopy);
    BigInt rhsTemp = reverse(rhsCopy);

    BigInt bigInt(largerSize * 2);
    computeMul(bigInt, lhsTemp, rhsTemp);
    BigInt result = reverse(bigInt);
    removeLeadingZeros(result);

    return result;
}


