#ifndef C_H
#define C_H

class B;

class C
{
public:
    C();
    bool equal() const;

private:
    B* m_p_b1 = nullptr;
    B* m_p_b2 = nullptr;
};

#endif // C_H
