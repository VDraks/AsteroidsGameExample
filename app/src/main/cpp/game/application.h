#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

namespace asteroids {

class ApplicationContext;

class Application {
public:
    static Application* self();

    void init(int width, int height);
    void update();
    void onTouch(int actionMask, float x, float y);
    void onPause();
    void onResume();

private:
    Application();
    Application(const Application&);
    Application& operator=(const Application&);

    std::unique_ptr<ApplicationContext> _context;
};

} // namespace asteroids

#endif // APPLICATION_H
