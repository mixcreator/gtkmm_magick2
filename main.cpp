#include <gtkmm.h>

#include "main_window.h"


int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(
        argc, argv, 
        "org.gtkmm.example.HelloApp"
    );
    MainWindow hw;
    return app->run(hw);
}