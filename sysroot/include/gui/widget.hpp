#ifndef WIDGET_HPP
#define WIDGET_HPP

namespace gui {
class widget {
public:
  widget()
    : x(0), y(0), width(0), height(0) {
  }

  widget(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {
  }

  bool test_hit(double mx, double my) {
    if (mx >= x && mx <= x + width) {
      if (my >= y && my <= y + height) {
        return true;
      }
    }
    return false;
  }

  virtual void on_click() {
    // to be overridden
  }

  int get_x() const {
    return x;
  }

  int get_y() const {
    return y;
  }

  int get_width() const {
    return width;
  }

  int get_height() const {
    return height;
  }

protected:
  int x, y, width, height;  
};
}

#endif