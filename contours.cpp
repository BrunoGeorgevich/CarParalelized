#include "contours.h"

Contours::Contours(vector<vector<Point> > contours, QObject *parent) : QObject(parent)
{
    m_data = contours;
}

vector<vector<Point> > Contours::data()
{
    return m_data;
}

void Contours::setData(vector<vector<Point> > contours)
{
    m_data = contours;
}

void Contours::append(vector<vector<Point> > contours)
{
    foreach (vector<Point> vp, contours) {
        m_data.push_back(vp);
    }
}

void Contours::clear()
{
    m_data.clear();
}
