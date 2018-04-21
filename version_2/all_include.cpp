#include "all_include.h"

double dms2Rad(double Dms)
{
    double Degree, Miniute;
    double Second;
    int Sign;
    double Rad;
    if (Dms >= 0)
        Sign = 1;
    else
        Sign = -1;
    Dms = fabs(Dms);
    Degree = floor(Dms);
    Miniute = floor(fmod(Dms * 100.0, 100.0));
    Second = fmod(Dms * 10000.0, 100.0);
    Rad = Sign * (Degree + Miniute / 60.0 + Second / 3600.0) * M_PI / 180.0;
    return Rad;
}

double dms2D(double Dms)
{
    double Degree, Miniute;
    double Second;
    int Sign;
    double D;
    if (Dms >= 0)
        Sign = 1;
    else
        Sign = -1;
    Dms = fabs(Dms);
    Degree = floor(Dms);
    Miniute = floor(fmod(Dms * 100.0, 100.0));
    Second = fmod(Dms * 10000.0, 100.0);
    D = Sign * (Degree + Miniute / 60.0 + Second / 3600.0);
    return D;
}

double D2Dms(double D)   //度化为度分秒
{
    double Degree, Miniute;
    double Second;
    int Sign;
    double Dms, Rad;
    if (D >= 0)
        Sign = 1;
    else
        Sign = -1;
    Rad = D*M_PI / 180;
    Rad = fabs(Rad * 180.0 / M_PI);
    Degree = floor(Rad);
    Miniute = floor(fmod(Rad * 60.0, 60.0));
    Second = fmod(Rad * 3600.0, 60.0);
    Dms = Sign * (Degree + Miniute / 100.0 + Second / 10000.0);
    return Dms;
}

Position calculateellipse2plane(double B, double L)
{
    double l = 0, Lo = 0, a0 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, n = 0, c = 0;
    double x = 0, y = 0;
    double m = 0, p = 0, q = 0;
    int N = 0, i = 0; //带号

    N = int(dms2D(L) / 6);
    Lo = 6 * N - 3;

    l = (dms2D(L) - dms2D(Lo)) * 3600 / rho;  //单位为"
    B = dms2Rad(B);
    L = dms2Rad(L);
    Lo = dms2Rad(Lo);

    l = pow(l, 2);
    c = pow(cos(B), 2);  //c=cos(B)的平方
    n = 6399698.902 - (21562.267 - (108.973 - 0.612*c)*c)*c;
    a0 = 32140.404 - (135.3302 - (0.7092 - 0.0040*c)*c)*c;
    a3 = (0.3333333 + 0.001123*c)*c - 0.1666667;
    a4 = (0.25 + 0.00252*c)*c - 0.04166;
    a5 = 0.0083 - (0.1667 - (0.1968 + 0.004*c)*c)*c;
    a6 = (0.166*c - 0.084)*c;
    //计算平面坐标，并化为国家统一坐标
    m = sin(B)*cos(B);
    p = 1 + (a3 + a5*l)*l;
    x = 6367558.4969*B - (a0 - (0.5 + (a4 + a6*l)*l)*l*n)*m;
    y = (p)*sqrt(l)*n*cos(B);
    y = y + 500000 + N * 1000000;
    return Position(x,y);
}

LatLon Calculateplane2ellipse(double x, double y)
{
    double l = 0, Lo = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, nf = 0, Z = 0, beta = 0, Bf = 0;
    double B = 0, L = 0;
    double c = 0, d = 0, m = 0;
    int N = 0;
    N = int(y / 1000000);
    Lo = 6 * N - 3;
    y = y - N * 1000000 - 500000;

    beta = x / 6367558.4969;  //单位为弧度
    c = cos(beta);
    c = pow(cos(beta), 2);   //c=cos(beta)的平方
    Bf = beta + (50221746 + (293622 + (2350 + 22 * c)*c)*c)*sin(beta)*cos(beta)*(1e-10);
    d = pow(cos(Bf), 2);     //d=cos(Bf)的平方
    nf = 6399698.902 - (21562.267 - (108.973 - 0.612*d)*d)*d;
    Z = y / (nf*cos(Bf));
    Z = pow(Z, 2);
    b2 = (0.5 + 0.003369*d)*sin(Bf)*cos(Bf);
    b3 = 0.333333 - (0.166667 - 0.001123*d)*d;
    b4 = 0.25 + (0.16161 + 0.00562*d)*d;
    b5 = 0.2 - (0.1667 - 0.0088*d)*d;

    //计算大地坐标B,L
    m = (1 - (b4 - 0.12*Z)*Z)*Z*b2;
    l = (1 - (b3 - b5*Z)*Z)*sqrt(Z)*rho;
    B = Bf*rho - m*rho;
    l = D2Dms(l / 3600);
    B = D2Dms(B / 3600);
    L = Lo + l;

    return LatLon(B,L);
}
