    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>
#include <QKeyEvent>
#include <QRectF>
#include <iomanip>
#include <iostream>
#include <utility>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;
class customPixel;


class customPixel{
public:

    static std::vector<std::vector<bool>> was1;
    static std::vector<std::vector<QColor>> matrix1;

    std::pair<int, int> coor() {
        return {x_, y_};
    }

    void drawPixel() {
        QRectF rect(x_, y_, 1, 1);
        if (was1[x_][y_]) {
            col_ = matrix1[x_][y_];
        } else {
            was1[x_][y_] = true;
            matrix1[x_][y_] = col_;
        }
        //col_ = throwRandomColor();
        QPen bordernow(col_);
        QBrush fillMe(col_);
        scene_->addRect(rect, bordernow, fillMe);
    }

    static QColor throwRandomColor(double strength = 1.0) {
        strength = std::min(strength, 1.0);
        int r = rand() % 256 * strength;
        int g = rand() % 256 * strength;
        int b = rand() % 256 * strength;
        return QColor(r, g, b);
    }

    customPixel(int x, int y, QColor col, QGraphicsScene * scene): x_(x), y_(y), col_(col){
        scene_ = scene;
        x_ = x;
        y_ = y;
        col_ = col;
        drawPixel();
        //int r = col_.red();
        //std::cout << r << std::endl;
    }

    customPixel(const customPixel& other) {
        x_ = other.x_;
        y_ = other.y_;
        col_ = other.col_;
    }

    customPixel(customPixel&& other){
        x_ = std::move(other.x_);
        y_ = std::move(other.y_);
        col_ = std::move(other.col_);
    }

    customPixel(int x, int y, QGraphicsScene * scene):  customPixel(x, y, customPixel::throwRandomColor(), scene){
        /*col_ = customPixel::throwRandomColor();
        *this = customPixel(x, y,  scene);*/
    }

    friend void swap(customPixel& a, customPixel& b) {
        std::swap(a.col_, b.col_);
        std::swap(a.x_, b.x_);
        std::swap(a.y_, b.y_);
    }

    customPixel operator=(customPixel other) {
        swap(*this, other);
        return *this;
    }

    customPixel(const customPixel & fi, const customPixel & se, QGraphicsScene *& scene, const int hight = customPixel::hight_) {
        int delta = 0;
        if (fi.x_ == se.x_) {
            x_ = fi.x_;
            delta = se.y_ - fi.y_;
            y_ = fi.y_ + delta / 2;
        } else  if (fi.y_ == se.y_){
            delta = se.x_ - fi.x_;
            x_ = fi.x_ + delta / 2;
            y_ = fi.y_;
        } else {
            delta = se.x_ - fi.x_;
            y_ = fi.y_ + (se.y_ - fi.y_) / 2;
            x_ = fi.x_ + delta / 2;
        }

        QColor tmp = customPixel::meanColor(delta / static_cast<double>(hight), fi.col_, se.col_);
        *this = customPixel(x_, y_, tmp, scene);

    }

    QColor getCol() {
        return col_;
    }

    std::vector<int> getRGBCol() {
        return {col_.red(), col_.green(), col_.blue()};
    }

    /*uint64_t x{}; // The state can be seeded with any value.
    uint64_t next()
    {
        uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
        z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
        return z ^ (z >> 31);
    }*/
    static QColor meanColor(double strength, std::vector<QColor> vi) {
        int R = 0, G = 0, B = 0;
        for (QColor fi : vi) {
            //std::cout << fi.red() << std::endl;
            int r, g, b;
            fi.getRgb(&r, &g, &b);
            R += r;
            G += g;
            B += b;
        }

        strength *= strength;

        int mod = 256;
        int r = rand() % mod * strength;
        int g = rand() % mod * strength;
        int b = rand() % mod * strength;
        R /= vi.size(); G /= vi.size(); B /= vi.size();
        //std::cout << R << std::endl;
        //R = std::min(R + r, 255); G = std::min(G + g, 255); B = std::min(B + b, 255);
        R = (R + r) % 256; G = (G + g) % 256; B = (B + b) % 256;
        return QColor(R, G, B);
    }

    QColor meanColor(double strength, QColor fi, QColor se) {
        return meanColor(strength, {fi, se});

    }


    static const int hight_ = 1024;



private:
    int x_, y_;
    QColor col_;
    QGraphicsScene * scene_;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void keyPressEvent( QKeyEvent * event );
    void design(int hight, customPixel lu, customPixel ld, customPixel ru, customPixel rd);

private:
    Ui::MainWindow *ui;
    QGraphicsScene * scene;
};
#endif // MAINWINDOW_H
