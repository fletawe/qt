/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmlgraphicsmousearea_p.h"
#include "qmlgraphicsmousearea_p_p.h"

#include "qmlgraphicsevents_p_p.h"

#include <QGraphicsSceneMouseEvent>

QT_BEGIN_NAMESPACE
static const int PressAndHoldDelay = 800;

QmlGraphicsDrag::QmlGraphicsDrag(QObject *parent)
: QObject(parent), _target(0), _axis(XandYAxis), _xmin(0), _xmax(0), _ymin(0), _ymax(0)
{
}

QmlGraphicsDrag::~QmlGraphicsDrag()
{
}

QmlGraphicsItem *QmlGraphicsDrag::target() const
{
    return _target;
}

void QmlGraphicsDrag::setTarget(QmlGraphicsItem *t)
{
    if (_target == t)
        return;
    _target = t;
    emit targetChanged();
}

QmlGraphicsDrag::Axis QmlGraphicsDrag::axis() const
{
    return _axis;
}

void QmlGraphicsDrag::setAxis(QmlGraphicsDrag::Axis a)
{
    if (_axis == a)
        return;
    _axis = a;
    emit axisChanged();
}

qreal QmlGraphicsDrag::xmin() const
{
    return _xmin;
}

void QmlGraphicsDrag::setXmin(qreal m)
{
    if (_xmin == m)
        return;
    _xmin = m;
    emit minimumXChanged();
}

qreal QmlGraphicsDrag::xmax() const
{
    return _xmax;
}

void QmlGraphicsDrag::setXmax(qreal m)
{
    if (_xmax == m)
        return;
    _xmax = m;
    emit maximumXChanged();
}

qreal QmlGraphicsDrag::ymin() const
{
    return _ymin;
}

void QmlGraphicsDrag::setYmin(qreal m)
{
    if (_ymin == m)
        return;
    _ymin = m;
    emit minimumYChanged();
}

qreal QmlGraphicsDrag::ymax() const
{
    return _ymax;
}

void QmlGraphicsDrag::setYmax(qreal m)
{
    if (_ymax == m)
        return;
    _ymax = m;
    emit maximumYChanged();
}

QmlGraphicsMouseAreaPrivate::~QmlGraphicsMouseAreaPrivate()
{
    delete drag;
}


/*!
    \qmlclass MouseRegion QmlGraphicsMouseRegion
    \since 4.7
    \brief The MouseArea item enables simple mouse handling.
    \inherits Item

    A MouseArea is typically used in conjunction with a visible item,
    where the MouseArea effectively 'proxies' mouse handling for that
    item. For example, we can put a MouseArea in a Rectangle that changes
    the Rectangle color to red when clicked:
    \snippet doc/src/snippets/declarative/mouseregion.qml 0

    Many MouseArea signals pass a \l {MouseEvent}{mouse} parameter that contains
    additional information about the mouse event, such as the position, button,
    and any key modifiers.

    Below we have the previous
    example extended so as to give a different color when you right click.
    \snippet doc/src/snippets/declarative/mouseregion.qml 1

    For basic key handling, see the \l {Keys}{Keys attached property}.

    MouseArea is an invisible item: it is never painted.

    \sa MouseEvent
*/

/*!
    \qmlsignal MouseArea::onEntered()

    This handler is called when the mouse enters the mouse region.

    By default the onEntered handler is only called while a button is
    pressed.  Setting hoverEnabled to true enables handling of
    onExited when no mouse button is pressed.

    \sa hoverEnabled
*/

/*!
    \qmlsignal MouseArea::onExited()

    This handler is called when the mouse exists the mouse region.

    By default the onExited handler is only called while a button is
    pressed.  Setting hoverEnabled to true enables handling of
    onExited when no mouse button is pressed.

    \sa hoverEnabled
*/

/*!
    \qmlsignal MouseArea::onPositionChanged(MouseEvent mouse)

    This handler is called when the mouse position changes.

    The \l {MouseEvent}{mouse} parameter provides information about the mouse, including the x and y
    position, and any buttons currently pressed.

    The \e accepted property of the MouseEvent parameter is ignored in this handler.

    By default the onPositionChanged handler is only called while a button is
    pressed.  Setting hoverEnabled to true enables handling of
    onPositionChanged when no mouse button is pressed.
*/

/*!
    \qmlsignal MouseArea::onClicked(mouse)

    This handler is called when there is a click. A click is defined as a press followed by a release,
    both inside the MouseArea (pressing, moving outside the MouseArea, and then moving back inside and
    releasing is also considered a click).

    The \l {MouseEvent}{mouse} parameter provides information about the click, including the x and y
    position of the release of the click, and whether the click wasHeld.

    The \e accepted property of the MouseEvent parameter is ignored in this handler.
*/

/*!
    \qmlsignal MouseArea::onPressed(mouse)

    This handler is called when there is a press.
    The \l {MouseEvent}{mouse} parameter provides information about the press, including the x and y
    position and which button was pressed.

    The \e accepted property of the MouseEvent parameter determines whether this MouseArea
    will handle the press and all future mouse events until release.  The default is to accept
    the event and not allow other MouseArea beneath this one to handle the event.  If \e accepted
    is set to false, no further events will be sent to this MouseArea until the button is next
    pressed.
*/

/*!
    \qmlsignal MouseArea::onReleased(mouse)

    This handler is called when there is a release.
    The \l {MouseEvent}{mouse} parameter provides information about the click, including the x and y
    position of the release of the click, and whether the click wasHeld.

    The \e accepted property of the MouseEvent parameter is ignored in this handler.
*/

/*!
    \qmlsignal MouseArea::onPressAndHold(mouse)

    This handler is called when there is a long press (currently 800ms).
    The \l {MouseEvent}{mouse} parameter provides information about the press, including the x and y
    position of the press, and which button is pressed.

    The \e accepted property of the MouseEvent parameter is ignored in this handler.
*/

/*!
    \qmlsignal MouseArea::onDoubleClicked(mouse)

    This handler is called when there is a double-click (a press followed by a release followed by a press).
    The \l {MouseEvent}{mouse} parameter provides information about the click, including the x and y
    position of the release of the click, and whether the click wasHeld.

    The \e accepted property of the MouseEvent parameter is ignored in this handler.
*/

/*!
    \internal
    \class QmlGraphicsMouseArea
    \brief The QmlGraphicsMouseArea class provides a simple mouse handling abstraction for use within Qml.

    \ingroup group_coreitems

    All QmlGraphicsItem derived classes can do mouse handling but the QmlGraphicsMouseArea class exposes mouse
    handling data as properties and tracks flicking and dragging of the mouse.

    A QmlGraphicsMouseArea object can be instantiated in Qml using the tag \l MouseArea.
 */
QmlGraphicsMouseArea::QmlGraphicsMouseArea(QmlGraphicsItem *parent)
  : QmlGraphicsItem(*(new QmlGraphicsMouseAreaPrivate), parent)
{
    Q_D(QmlGraphicsMouseArea);
    d->init();
}

QmlGraphicsMouseArea::~QmlGraphicsMouseArea()
{
}

/*!
    \qmlproperty real MouseArea::mouseX
    \qmlproperty real MouseArea::mouseY
    These properties hold the coordinates of the mouse.

    If the hoverEnabled property is false then these properties will only be valid
    while a button is pressed, and will remain valid as long as the button is held
    even if the mouse is moved outside the region.

    If hoverEnabled is true then these properties will be valid:
    \list
        \i when no button is pressed, but the mouse is within the MouseArea (containsMouse is true).
        \i if a button is pressed and held, even if it has since moved out of the region.
    \endlist

    The coordinates are relative to the MouseArea.
*/
qreal QmlGraphicsMouseArea::mouseX() const
{
    Q_D(const QmlGraphicsMouseArea);
    return d->lastPos.x();
}

qreal QmlGraphicsMouseArea::mouseY() const
{
    Q_D(const QmlGraphicsMouseArea);
    return d->lastPos.y();
}

/*!
    \qmlproperty bool MouseArea::enabled
    This property holds whether the item accepts mouse events.
*/
bool QmlGraphicsMouseArea::isEnabled() const
{
    Q_D(const QmlGraphicsMouseArea);
    return d->absorb;
}

void QmlGraphicsMouseArea::setEnabled(bool a)
{
    Q_D(QmlGraphicsMouseArea);
    if (a != d->absorb) {
        d->absorb = a;
        emit enabledChanged();
    }
}
/*!
    \qmlproperty MouseButtons MouseArea::pressedButtons
    This property holds the mouse buttons currently pressed.

    It contains a bitwise combination of:
    \list
    \o Qt.LeftButton
    \o Qt.RightButton
    \o Qt.MidButton
    \endlist

    The code below displays "right" when the right mouse buttons is pressed:
    \code
    Text {
        text: mr.pressedButtons & Qt.RightButton ? "right" : ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        MouseArea {
            id: mr
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            anchors.fill: parent
        }
    }
    \endcode

    \sa acceptedButtons
*/
Qt::MouseButtons QmlGraphicsMouseArea::pressedButtons() const
{
    Q_D(const QmlGraphicsMouseArea);
    return d->lastButtons;
}

void QmlGraphicsMouseArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    d->moved = false;
    if (!d->absorb)
        QmlGraphicsItem::mousePressEvent(event);
    else {
        d->longPress = false;
        d->saveEvent(event);
        if (d->drag) {
            d->dragX = drag()->axis() & QmlGraphicsDrag::XAxis;
            d->dragY = drag()->axis() & QmlGraphicsDrag::YAxis;
        }
        d->dragged = false;
        setHovered(true);
        d->start = event->pos();
        d->startScene = event->scenePos();
        // we should only start timer if pressAndHold is connected to.
        if (d->isConnected("pressAndHold(QmlGraphicsMouseEvent*)"))
            d->pressAndHoldTimer.start(PressAndHoldDelay, this);
        setKeepMouseGrab(false);
        event->setAccepted(setPressed(true));
    }
}

void QmlGraphicsMouseArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->absorb) {
        QmlGraphicsItem::mouseMoveEvent(event);
        return;
    }

    d->saveEvent(event);

    // ### we should skip this if these signals aren't used
    // ### can GV handle this for us?
    bool contains = boundingRect().contains(d->lastPos);
    if (d->hovered && !contains)
        setHovered(false);
    else if (!d->hovered && contains)
        setHovered(true);

    if (d->drag && d->drag->target()) {
        if (!d->moved) {
            if (d->dragX) d->startX = drag()->target()->x();
            if (d->dragY) d->startY = drag()->target()->y();
        }

        QPointF startLocalPos;
        QPointF curLocalPos;
        if (drag()->target()->parent()) {
            startLocalPos = drag()->target()->parentItem()->mapFromScene(d->startScene);
            curLocalPos = drag()->target()->parentItem()->mapFromScene(event->scenePos());
        } else {
            startLocalPos = d->startScene;
            curLocalPos = event->scenePos();
        }

        const int dragThreshold = QApplication::startDragDistance();
        qreal dx = qAbs(curLocalPos.x() - startLocalPos.x());
        qreal dy = qAbs(curLocalPos.y() - startLocalPos.y());
        if ((d->dragX && !(dx < dragThreshold)) || (d->dragY && !(dy < dragThreshold)))
            d->dragged = true;
        if (!keepMouseGrab()) {
            if ((!d->dragY && dy < dragThreshold && d->dragX && dx > dragThreshold)
                || (!d->dragX && dx < dragThreshold && d->dragY && dy > dragThreshold)
                || (d->dragX && d->dragY)) {
                setKeepMouseGrab(true);
            }
        }

        if (d->dragX) {
            qreal x = (curLocalPos.x() - startLocalPos.x()) + d->startX;
            if (x < drag()->xmin())
                x = drag()->xmin();
            else if (x > drag()->xmax())
                x = drag()->xmax();
            drag()->target()->setX(x);
        }
        if (d->dragY) {
            qreal y = (curLocalPos.y() - startLocalPos.y()) + d->startY;
            if (y < drag()->ymin())
                y = drag()->ymin();
            else if (y > drag()->ymax())
                y = drag()->ymax();
            drag()->target()->setY(y);
        }
    }
    d->moved = true;
    QmlGraphicsMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, false, d->longPress);
    emit positionChanged(&me);
}


void QmlGraphicsMouseArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->absorb) {
        QmlGraphicsItem::mouseReleaseEvent(event);
    } else {
        d->saveEvent(event);
        setPressed(false);
        // If we don't accept hover, we need to reset containsMouse.
        if (!acceptHoverEvents())
            setHovered(false);
        setKeepMouseGrab(false);
    }
}

void QmlGraphicsMouseArea::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->absorb) {
        QmlGraphicsItem::mouseDoubleClickEvent(event);
    } else {
        QmlGraphicsItem::mouseDoubleClickEvent(event);
        if (event->isAccepted()) {
            // Only deliver the event if we have accepted the press.
            d->saveEvent(event);
            QmlGraphicsMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, true, false);
            emit this->doubleClicked(&me);
        }
    }
}

void QmlGraphicsMouseArea::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->absorb)
        QmlGraphicsItem::hoverEnterEvent(event);
    else
        setHovered(true);
}

void QmlGraphicsMouseArea::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->absorb) {
        QmlGraphicsItem::hoverEnterEvent(event);
    } else {
        d->lastPos = event->pos();
        QmlGraphicsMouseEvent me(d->lastPos.x(), d->lastPos.y(), Qt::NoButton, d->lastButtons, d->lastModifiers, false, d->longPress);
        emit positionChanged(&me);
    }
}

void QmlGraphicsMouseArea::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->absorb)
        QmlGraphicsItem::hoverLeaveEvent(event);
    else
        setHovered(false);
}

bool QmlGraphicsMouseArea::sceneEvent(QEvent *event)
{
    bool rv = QmlGraphicsItem::sceneEvent(event);
    if (event->type() == QEvent::UngrabMouse) {
        Q_D(QmlGraphicsMouseArea);
        if (d->pressed) {
            // if our mouse grab has been removed (probably by Flickable), fix our
            // state
            d->pressed = false;
            setKeepMouseGrab(false);
            emit pressedChanged();
            //emit hoveredChanged();
        }
    }
    return rv;
}

void QmlGraphicsMouseArea::timerEvent(QTimerEvent *event)
{
    Q_D(QmlGraphicsMouseArea);
    if (event->timerId() == d->pressAndHoldTimer.timerId()) {
        d->pressAndHoldTimer.stop();
        if (d->pressed && d->dragged == false && d->hovered == true) {
            d->longPress = true;
            QmlGraphicsMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, false, d->longPress);
            emit pressAndHold(&me);
        }
    }
}

/*!
    \qmlproperty bool MouseArea::hoverEnabled
    This property holds whether hover events are handled.

    By default, mouse events are only handled in response to a button event, or when a button is
    pressed.  Hover enables handling of all mouse events even when no mouse button is
    pressed.

    This property affects the containsMouse property and the onEntered, onExited and onPositionChanged signals.
*/

/*!
    \qmlproperty bool MouseArea::containsMouse
    This property holds whether the mouse is currently inside the mouse region.

    \warning This property is not updated if the region moves under the mouse: \e containsMouse will not change.
    In addition, if hoverEnabled is false, containsMouse will only be valid when the mouse is pressed.
*/
bool QmlGraphicsMouseArea::hovered() const
{
    Q_D(const QmlGraphicsMouseArea);
    return d->hovered;
}

/*!
    \qmlproperty bool MouseArea::pressed
    This property holds whether the mouse region is currently pressed.
*/
bool QmlGraphicsMouseArea::pressed() const
{
    Q_D(const QmlGraphicsMouseArea);
    return d->pressed;
}

void QmlGraphicsMouseArea::setHovered(bool h)
{
    Q_D(QmlGraphicsMouseArea);
    if (d->hovered != h) {
        d->hovered = h;
        emit hoveredChanged();
        d->hovered ? emit entered() : emit exited();
    }
}

/*!
    \qmlproperty Qt::MouseButtons MouseArea::acceptedButtons
    This property holds the mouse buttons that the mouse region reacts to.

    The available buttons are:
    \list
    \o Qt.LeftButton
    \o Qt.RightButton
    \o Qt.MidButton
    \endlist

    To accept more than one button the flags can be combined with the
    "|" (or) operator:

    \code
    MouseArea { acceptedButtons: Qt.LeftButton | Qt.RightButton }
    \endcode

    The default is to accept the Left button.
*/
Qt::MouseButtons QmlGraphicsMouseArea::acceptedButtons() const
{
    return acceptedMouseButtons();
}

void QmlGraphicsMouseArea::setAcceptedButtons(Qt::MouseButtons buttons)
{
    if (buttons != acceptedMouseButtons()) {
        setAcceptedMouseButtons(buttons);
        emit acceptedButtonsChanged();
    }
}

bool QmlGraphicsMouseArea::setPressed(bool p)
{
    Q_D(QmlGraphicsMouseArea);
    bool isclick = d->pressed == true && p == false && d->dragged == false && d->hovered == true;

    if (d->pressed != p) {
        d->pressed = p;
        QmlGraphicsMouseEvent me(d->lastPos.x(), d->lastPos.y(), d->lastButton, d->lastButtons, d->lastModifiers, isclick, d->longPress);
        if (d->pressed) {
            emit positionChanged(&me);
            emit pressed(&me);
        } else {
            emit released(&me);
            if (isclick)
                emit clicked(&me);
        }

        emit pressedChanged();
        return me.isAccepted();
    }
    return false;
}

QmlGraphicsDrag *QmlGraphicsMouseArea::drag()
{
    Q_D(QmlGraphicsMouseArea);
    if (!d->drag)
        d->drag = new QmlGraphicsDrag;
    return d->drag;
}

/*!
    \qmlproperty Item MouseArea::drag.target
    \qmlproperty Axis MouseArea::drag.axis
    \qmlproperty real MouseArea::drag.minimumX
    \qmlproperty real MouseArea::drag.maximumX
    \qmlproperty real MouseArea::drag.minimumY
    \qmlproperty real MouseArea::drag.maximumY

    drag provides a convenient way to make an item draggable.

    \list
    \i \c target specifies the item to drag.
    \i \c axis specifies whether dragging can be done horizontally (XAxis), vertically (YAxis), or both (XandYAxis)
    \i the minimum and maximum properties limit how far the target can be dragged along the corresponding axes.
    \endlist

    The following example uses drag to reduce the opacity of an image as it moves to the right:
    \snippet doc/src/snippets/declarative/drag.qml 0
*/

QT_END_NAMESPACE