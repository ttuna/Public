#ifndef A_H
#define A_H

class QString;

class A
{
public:
    A();

    QString getString() const;
    void setString(const QString& in_string);

private:
    QString* m_p_string = nullptr;
};

#endif // A_H
