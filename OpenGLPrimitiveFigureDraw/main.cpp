#include <iostream>
#include <vector>

#include <QApplication>
#if QT_VERSION_MAJOR >= 6
#include <QtOpenGLWidgets/QOpenGLWidget>
#else
#include <QOpenGLWidget>
#endif
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugMessage>
#include <QDebug>
#include <GL/glu.h>
#include <vector>

struct vec3{float x,y,z;};
struct vec4{float x,y,z,w;};

const char* vertex_shader_src = "#version 330\n"
                                "layout (location=7) in vec3 pos;"
                                "layout (location=8) in vec4 color;"
                                "out vec4 vert_color;"
                                "void main() {"
                                "   gl_Position = vec4(pos,1.0);"
                                "   vert_color = color;"
                                "}";
const char* fragment_shader_src = "#version 330\n"
                                  "in vec4 vert_color;"
                                  "out vec4 frag_color;"
                                  "void main() {"
                                  "   frag_color = vert_color;"
                                  "};";



/**
 * @brief PRINT_GL_ERRORS - funkcja przeglada stos bledow opengl, jezeli na stosie sa bledy, drukuje je na konsoli
 * @param mess - dowolny string, ktory bedzie wyswietlony przed opisem bledu
 */
void PRINT_GL_ERRORS(QString mess)
{
    GLenum err;
    while( (err=glGetError()) != GL_NO_ERROR)
        qDebug() << "GL ERROR: " << mess << err << (const char*)gluErrorString(err) << "(" << err <<")";
}

/**
 * @brief handleLoggedMessage - nowszy sposób obsługi bledow wspomagany przez Qt
 */
void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    if (debugMessage.id() == 131185) return; //for nvidia buffer create message
    qDebug() << "GL message: " << debugMessage;
}


/// \brief The Window class - klasa, ktora stanowi okno naszej aplikacji
/// i zawiera w sobie kontekst opengl-a.
class Window : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    //!Pomocnicza funkcja wyswietlajaca potencjalne bledy kompilacji shadera
    bool checkShaderStatus(GLuint shader)
    {
        GLint stat;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &stat);
        if(stat == GL_FALSE)
        {
            GLchar infoLog[10240];
            glGetShaderInfoLog(shader, 10240, NULL, infoLog);
            qDebug() << "Shader ERROR: " << infoLog;
            return false;
        }
        return true;
    }

    //!Pomocnicza funkcja wyswietlajaca potencjalne bledy linkowania programu shadera
    bool checkProgramStatus(GLuint program)
    {
        GLint stat;
        glGetProgramiv(program, GL_LINK_STATUS, &stat);
        if(stat == GL_FALSE)
        {
            GLchar infoLog[10240];
            glGetProgramInfoLog(program, 10240, NULL, infoLog);
            qDebug() << "Program ERROR: " << infoLog;
            return false;
        }
        return true;
    }

    std::vector<vec3> pos;
    std::vector<vec4> colors;



    GLuint pos_buffer;
    int pos_index;
    int color_index;
    GLuint color_buffer;
    GLuint vba;

    GLuint fragment_shader;
    GLuint shader_program;
    GLuint vertex_shader;



protected:
    /**
     * @brief initializeGL - reimplementacja metody; wywolywana po konstrukcji obiektu klasy
     * ale przed pierwszym wywolaniem resizeGL() i paintGL().
     * W tej funkcji mamy gwarancje, ze kontekst opengl jest juz utworzony.
     * To jest odpowiednie miejsce do setup-u opengla i sceny.
     */
    void initializeGL() override
    {
        //!Wywolanie metody z klasy QOpenGLFunctions; niezbedne do utawienia odpowiednich wskaznikow do funkcji opengl
        initializeOpenGLFunctions();

        pos = {{0,0,0},{-0.5,0,0},{0,-0.5,0},{0,0,0},{1,0,0},{0,1,0}};

        colors = {{1,0,0,1},{0,1,0,1},{0,0,1,1},{1,0,0,1},{0,1,0,1},{0,0,1,1}};

        glGenVertexArrays(1,&vba);
        glBindVertexArray(vba);

        pos_index = 7;
        glGenBuffers(1,&pos_buffer); // nie tworzy tylko mowie ze mozemy uzyc tej nazwy
        glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);// STWORZENIE I ZWIAZANIE GO Z PUNKTEM WIZANIA
        glBufferData(GL_ARRAY_BUFFER, pos.size()*sizeof(pos[0]),pos.data(),GL_STATIC_DRAW); //skopiowanie danmych
        glEnableVertexAttribArray(pos_index);
        glVertexAttribPointer(pos_index, 3, GL_FLOAT, GL_FALSE,0,0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        color_index = 8;
        //kolorki
        glGenBuffers(1,&color_buffer); // nie tworzy tylko mowie ze mozemy uzyc tej nazwy zal.pl
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);// STWORZENIE I ZWIAZANIE GO Z PUNKTEM WIZANIA
        glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(colors[0]),colors.data(),GL_STATIC_DRAW); //skopiowanie danmych
        glEnableVertexAttribArray(color_index);
        glVertexAttribPointer(color_index, 4, GL_FLOAT, GL_FALSE,0,0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        shader_program = glCreateProgram();

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader,1,(const GLchar**)&vertex_shader_src,0);
        glCompileShader(vertex_shader);
        checkShaderStatus(vertex_shader);
        glAttachShader(shader_program, vertex_shader);
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader,1,(const GLchar**)&fragment_shader_src,0);
        glCompileShader(fragment_shader);
        checkShaderStatus(fragment_shader);
        glAttachShader(shader_program, fragment_shader);

        glLinkProgram(shader_program);
        checkProgramStatus(shader_program);









        //!Wypisanie wersji opengl
        qDebug() << (const char*)glGetString(GL_VERSION);

        PRINT_GL_ERRORS("initializeGL() at line " + QString::number(__LINE__));
    }

    /**
     * @brief paintGL - reimplementacja metody; wywolywana zawsze gdy jest potrzeba
     * odrysowania widgetu. Nie wywolujemy jej nigdy jawnie.
     * Jezeli jest potrzeba odrysowania sceny, wywolujemy te funkcje niejawnie,
     * poprzez metode update()
     */
    void paintGL() override
    {
        //!Kolor czyszczenia bufora kolorów, (r,g,b,a) in <0.0; 1.0>
        glClearColor(0.5, 0.5, 1.0, 1.0);

        //!Wyczyszczenie bufora kolorow i glebi
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vba); //vao
        glDrawArrays(GL_TRIANGLES,0,pos.size());
        glBindVertexArray(0);
        glUseProgram(0);

        PRINT_GL_ERRORS("paintGL() at line " + QString::number(__LINE__));
    }

    /**
     * @brief resizeGL - reimplementacja metody; wywolywana jest zawsze gdy
     * okienko pojawia się lub zmienia swoj rozmiar.
     * Dobre miejsce do wywolania niezbednych funkcji, do setupu sceny zaleznych
     * od wielkosci okna, np. glViewport() lub aktualizacji macierzy projekcji.
     * @param w - w tym parametrze bedzie nowa szerokosc okna
     * @param h - w tym parametrze bedzie nowa wysokosc okna
     */
    void resizeGL(int w, int h) override
    {
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //!Utworzenie i wyswietlenie okna aplikacji
    Window w;
    w.show();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger();
    logger->initialize();
    QObject::connect(logger, &QOpenGLDebugLogger::messageLogged, handleLoggedMessage);
    logger->startLogging();

    return a.exec();
}
