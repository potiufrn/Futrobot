#include "calibrador.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include "pixmap_label.h"
#include "../calibrador.ui.h"
/*
 *  Constructs a calibrador as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
calibrador::calibrador(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
calibrador::~calibrador()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void calibrador::languageChange()
{
    retranslateUi(this);
}

