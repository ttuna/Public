#ifndef B_H
#define B_H

class A;
class QString;

class B
{
public:
    B();

    QString getAString() const;
    int getInt() const;

private:
    A* m_p_a = nullptr;
    int m_int = 0;
};

#endif // B_H
