// Copyright (C) 2010 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.


# include <shared/src/image_viewer.hh>
# include <shared/src/crop_item.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      image_viewer::image_viewer(QWidget *parent)
	: QWidget(parent), image_(0), selection_(0), angle_(0),
	  zoom_fixed_(false)
      {
	setupUi(this);

	connect(viewer_->verticalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(move_vertical_sliders(int)));
	connect(viewer_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(move_horizontal_sliders(int)));

	setup_scene();

	slider->setMinimum(0);
	visible_slider(false);

	set_selection_enabled(false);
	set_rotation_enabled(false);

	// Set cache limit to 20Mb.
	QPixmapCache::setCacheLimit(20480);
      }



      image_viewer::~image_viewer()
      {

      }

      unsigned image_viewer::current_slice() const
      {
	return slider->value();
      }

      internal::interactiveScene * image_viewer::scene()
      {
	return static_cast<internal::interactiveScene *>(viewer_->scene());
      }

      const internal::interactiveScene * image_viewer::scene() const
      {
	return static_cast<const internal::interactiveScene *>(viewer_->scene());
      }

      // Private members

      void image_viewer::setup_scene()
      {
	QGraphicsScene *scene = new internal::interactiveScene();
	viewer_->setScene(scene);
	viewer_->show();
      }


      void image_viewer::draw_image(const QPixmap& pixmap)
      {
	draw_image(pixmap, QPoint(0, 0));
      }


      void image_viewer::draw_image(const QPixmap& pixmap,
				    const QPoint& origin)
      {
        bool restore_selection = false;

	if (image_ != 0)
	{
	  viewer_->scene()->removeItem(image_);
	  delete image_;

	  if (selection_)
	  {
	    selection_ = 0;
	    restore_selection = true;
	  }
	}

	image_ = viewer_->scene()->addPixmap(pixmap);
	image_->setCacheMode(QGraphicsItem::DeviceCoordinateCache,
			     QSize(100,100));
	viewer_->scene()->setSceneRect(image_->boundingRect());

	if (pixmap.width() > viewer_->maximumViewportSize().width())
	  viewer_->fitInView(image_->boundingRect(), Qt::KeepAspectRatio);

	enable_widgets(true);

	image_->translate(origin.x(), origin.y());


	viewer_->setSceneRect(image_->sceneBoundingRect());

	// Restore selection mode if needed
	setup_selection_tool(restore_selection);
      }


      void image_viewer::resize_image(const QRectF& rect)
      {
	if (image_ != 0)
	  viewer_->fitInView(rect, Qt::KeepAspectRatio);
      }

      void image_viewer::resizeEvent(QResizeEvent * event)
      {
	if (zoom_fixed_)
	  on_zoomFixed_clicked();
	else
	  on_zoomOriginal_clicked();

	event->ignore();
      }

      void image_viewer::keyPressEvent(QKeyEvent *event)
      {
	if (event->text() == QString("+"))
	{
	  on_zoomIn_clicked();
	  event->accept();
	}
	else if (event->text() == QString("-"))
	{
	  on_zoomOut_clicked();
	  event->accept();
	}
	else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
	  if (event->modifiers() == Qt::ControlModifier)
	    on_zoomFixed_clicked();
	  else
	    on_zoomOriginal_clicked();
	  event->accept();
	}
	else
	  event->ignore();
      }


      // Private slots

      void image_viewer::visible_slider(bool b)
      {
	slider->setVisible(b);
	label_2->setVisible(b);
	frame_nb->setVisible(b);
      }

      void image_viewer::move_vertical_sliders(int value)
      {
	viewer_->verticalScrollBar()->setValue(value);
      }


      void image_viewer::move_horizontal_sliders(int value)
      {
	viewer_->horizontalScrollBar()->setValue(value);
      }


      void image_viewer::on_slider_valueChanged(int sli)
      {
	frame_nb->setText(QString("%1/%2").arg(sli).arg(slider->maximum()));
	emit slider_valueChanged(sli);
      }

      void image_viewer::update_image(const mln::image2d<dsp_data_t>& ima)
      {
	draw_image(ima);
      }

      void image_viewer::on_rotate_cw_btn_clicked()
      {
	angle_ += 90 % 360;
	image_->rotate(90);
	viewer_->setSceneRect(image_->sceneBoundingRect());
      }

      void image_viewer::on_rotate_ccw_btn_clicked()
      {
	angle_ -= 90 % 360;
	image_->rotate(-90);
	viewer_->setSceneRect(image_->sceneBoundingRect());
      }

      void image_viewer::on_zoomIn_clicked()
      {
	viewer_->scale(1.2, 1.2);
      }

      void image_viewer::on_zoomOut_clicked()
      {
	viewer_->scale(1 / 1.2, 1 / 1.2);
      }

      void image_viewer::on_zoomFixed_clicked()
      {
	zoom_fixed_ = true;
	if (image_ != 0)
	  resize_image(image_->boundingRect());
      }

      void image_viewer::on_zoomOriginal_clicked()
      {
	zoom_fixed_ = false;
	if (image_ != 0)
	  resize_image(viewer_->viewport()->geometry());
      }


      void image_viewer::new_mouse_selection_slot(const QPointF& p)
      {
	if (selection_)
	{
	  if (selection_->cropRect().contains(p))
	    return;
	  else
	  {
	    setup_selection_tool(false);
	    return;
	  }
	}

	setup_selection_tool(false);   // Remove previous selection.
	setup_selection_tool(true, p); // Create new selection.
      }


      void image_viewer::new_mouse_released_slot(const QPointF&)
      {
	if (selection_ && !selection_->cropRect().isValid()) // CAS POURRI!!
	  setup_selection_tool(false);
      }


      void image_viewer::clear()
      {
	visible_slider(false);
	if (image_ != 0 && image_->scene() == viewer_->scene())
	{
	  viewer_->scene()->removeItem(image_);
	  delete image_;
	  image_ = 0;
	}
      }

      void image_viewer::setup_selection_tool(bool b, const QPointF& p)
      {
	if (b)
	{
	  if (viewer_->scene() && selection_ == 0)
	  {
	    if (!p.isNull())
	      selection_ = new crop_item(p, image_);
	    else
	      selection_ = new crop_item(image_);

	    // Enable cropping on double click.
	    connect(selection_, SIGNAL(ready_for_crop()),
		    this, SIGNAL(ready_for_crop()));
	  }
	}
	else
	{
	  delete selection_;
	  selection_ = 0;
	}
      }


      void image_viewer::set_selection_enabled(bool b)
      {
	if (b)
	{
	  connect(viewer_->scene(), SIGNAL(mouse_pressed(const QPointF&)),
		  this, SLOT(new_mouse_selection_slot(const QPointF&)));
	  connect(viewer_->scene(), SIGNAL(mouse_released(const QPointF&)),
		  this, SLOT(new_mouse_released_slot(const QPointF&)));
	}
	else
	{
	  disconnect(viewer_->scene(), SIGNAL(mouse_pressed(const QPointF&)),
		     this, SLOT(new_mouse_selection_slot(const QPointF&)));
	  disconnect(viewer_->scene(), SIGNAL(mouse_released(const QPointF&)),
		     this, SLOT(new_mouse_released_slot(const QPointF&)));
	}
      }


      void image_viewer::set_rotation_enabled(bool b)
      {
	angle_ = 0;
	rotate_lbl->setVisible(b);
	rotate_cw_btn->setVisible(b);
	rotate_ccw_btn->setVisible(b);
	vline->setVisible(b);
      }


      bool image_viewer::has_selection() const
      {
	return selection_;
      }


      QRectF image_viewer::selection() const
      {
	static QRectF invalid_selection;

	if (has_selection())
	  return selection_->cropRect();

	return invalid_selection;
      }


      bool image_viewer::has_rotation() const
      {
	return rotate_lbl->isVisible();
      }


      qreal image_viewer::rotation() const
      {
	mln_assertion(has_rotation());

	return angle_;
      }


      QPixmap image_viewer::image() const
      {
	return image_->pixmap();
      }

      // Public slots

      void image_viewer::set_image_layer_count(unsigned nslis)
      {
	visible_slider(nslis > 1);

	slider->setMaximum(nslis - 1);

	unsigned sli = current_slice();
	if (current_slice() >= nslis)
	  sli = 0;

	slider->setSliderPosition(sli);
	on_slider_valueChanged(sli);
      }


      void image_viewer::enable_widgets(bool b)
      {
	zoomLabel->setEnabled(b);
	zoomIn->setEnabled(b);
	zoomOut->setEnabled(b);
	zoomFixed->setEnabled(b);
	zoomOriginal->setEnabled(b);

	rotate_ccw_btn->setEnabled(b);
	rotate_cw_btn->setEnabled(b);
	rotate_lbl->setEnabled(b);
      }


    } // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo