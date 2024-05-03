#include "Particle.h"

using namespace sf;

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition): m_A(2, numPoints)
{
    m_ttl = TTL;
    m_numPoints = numPoints;
    m_radiansPerSec = ((float)rand() / (RAND_MAX) * M_PI);
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
    Vector2i point = (mouseClickPosition);
    m_centerCoordinate = target.mapPixelToCoords(point, m_cartesianPlane);
    int numX = ((rand() % (500 - 100 + 1)) + 100);
    int numY = ((rand() % (500 - 100 + 1)) + 100);
    m_vx = numX;
    if (numX % 2 != 0)
    {
        numX * -1;
        m_vx = numX;
    }
    m_vy = numY;
    Uint8 r = ((rand() % (255 - 0 + 1)) + 0);
    Uint8 g= ((rand() % (255 - 0 + 1)) + 0);
    Uint8 b= ((rand() % (255 - 0 + 1)) + 0);
    m_color1 = Color::White;
    m_color2 = Color::Color(r, g, b);

    float theta = ((float)rand() / (RAND_MAX)*M_PI / 2);
    float dTheta = 2 * M_PI / (numPoints - 1);
    for (int j = 0; j < numPoints; j++)
    {
        r = ((rand() % (80 - 20 + 1)) + 20);
        float dx = r * cos(theta);
        float dy = r * sin(theta);
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        theta += dTheta;
    }
    cout << "** " << m_A.getRows() << " rows in particle constructor" << endl;
    cout << "** " << m_A.getCols() << " cols in particle constructor" << endl;
}
void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(TriangleFan, m_numPoints + 1);
    Vector2f center = Vector2f(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));
    lines[0].position = center;
    lines[0].color = m_color1;
    for (int j = 1; j <= m_numPoints; j++)
    {
        lines[j].position = Vector2f(target.mapCoordsToPixel({ (float)m_A(0, j - 1), (float)m_A(1, j - 1) }, m_cartesianPlane));
        lines[j].color = m_color2;
    }
    target.draw(lines);
}
void Particle::update(float dt)
{
    m_ttl = m_ttl - dt;
    rotate(dt * m_radiansPerSec);
    scale(SCALE);
    float dx = m_vx * dt;
    m_vy = m_vy - (G * dt);
    float dy = m_vy * dt;
    translate(dx, dy);
}
float Particle::getTTL()
{
    return m_ttl;
}


bool Particle::almostEqual(double a, double b, double eps)      
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    
    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    cout << "** " << initialCoords.getRows() << " rows in particle tester" << endl;
    cout << "** " << initialCoords.getCols() << " cols in particle tester" << endl;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        cout << "** begin for loop j at " << j << endl;
        cout << "** bool value " << rotationPassed << endl;
        //cout << "** " << almostEqual(m_A(0, j), -initialCoords(1, j)) << ", " << almostEqual(m_A(1, j), initialCoords(0, j)) << endl;
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "** here inside if" << endl;

            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
        cout << "** end of for loop j at " << j << endl << endl;

    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }
    /*
    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;*/
}

void Particle::rotate(double theta)
{
    cout << "** begin rotate function" << endl;
    Vector2f temp = m_centerCoordinate;
    cout << "** before translate" << -m_centerCoordinate.x << ", " << -m_centerCoordinate.y << endl;
    //(-m_centerCoordinate.x, -m_centerCoordinate.y);
    cout << "** after translate" << -m_centerCoordinate.x << ", " << -m_centerCoordinate.y << endl;
    RotationMatrix R = theta;
    m_A = R * m_A;
    //translate(temp.x, temp.y);
    cout << "** end rotate function" << endl;

}
void Particle::scale(double c)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    ScalingMatrix S(c);
    m_A = S * m_A;
    translate(temp.x, temp.y);
}
void Particle::translate(double xShift, double yShift)
{
    cout << "** begin translate function" << endl;
    TranslationMatrix T(xShift, yShift, 2);
    m_A = T + m_A;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
    cout << "** end translate function" << endl;
}