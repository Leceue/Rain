//#ifndef TRIANGLE_H
//#define TRIANGLE_H

//#include <QWidget>
//#include <QtOpenGLWidgets/QOpenGLWidget>
//#include <QOpenGLExtraFunctions>
//#include <QDebug>

//QT_BEGIN_NAMESPACE
//namespace Ui { class Triangle; }
//QT_END_NAMESPACE

//class Triangle : public QOpenGLWidget, protected QOpenGLExtraFunctions
//{
//    Q_OBJECT

//public:
//    Triangle();
//    ~Triangle();

//protected:
//    virtual void initializeGL();
//    virtual void resizeGL(int w,int h);
//    virtual void paintGL();

//private:
//    GLuint shaderPrograme;
//};
//#endif // TRIANGLE_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QDebug>

class Triangle : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    Triangle();
    ~Triangle();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
private:
    GLuint shaderProgram;
};

#endif // WIDGET_H
