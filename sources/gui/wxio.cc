//
// FILE: wxio.cc -- Implementation of more complicated I/O functions
//
// $Id$
//

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // WX_PRECOMP
#include "wxio.h"

static char wxio_buffer[100];

class gWxIOFrame : public wxFrame {
private:
  gWxOutput *parent;

public:
  wxTextCtrl *f;

  gWxIOFrame(gWxOutput *parent, const char *label);
  bool OnClose(void);
};


gWxIOFrame::gWxIOFrame(gWxOutput *parent_, const char *label)
  : wxFrame(NULL, -1, (char *)label, wxPoint(0, 0), wxSize(200, 200)),
    parent(parent_)
{
  f = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(200, 200),
		     wxTE_READONLY | wxTE_MULTILINE);
  Show(TRUE);
}


bool gWxIOFrame::OnClose(void)
{
  parent->OnClose();
  return TRUE;
}


//************************************ G WX OUTPUT ****************************
gWxOutput::gWxOutput(const char *label_)
{
    if (label_) 
        label = copystring(label_);
    else 
        label = 0;

    frame = 0;
    Width = 0;
    Prec  = 6;
    Represent = 'f';
}


gWxOutput::~gWxOutput()
{
    if (frame) 
        delete frame;

    if (label)
        delete [] label;
}


int gWxOutput::GetWidth(void) const
{
    return Width;
}


gOutput &gWxOutput::SetWidth(int w)
{
    Width = w;
    return *this;
}


int gWxOutput::GetPrec(void) const
{
    return Prec;
}


gOutput &gWxOutput::SetPrec(int p)
{
    Prec = p;
    return *this;
}


gOutput &gWxOutput::SetExpMode(void)
{
    Represent = 'e';
    return *this;
}


gOutput &gWxOutput::SetFloatMode(void)
{
    Represent = 'f';
    return *this;
}


char gWxOutput::GetRepMode(void) const
{
    return Represent;
}


gOutput &gWxOutput::operator<<(int x)
{
    if (!frame) 
        MakeFrame();

    sprintf(wxio_buffer, "%*d", Width,  x);
    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(unsigned int x)
{
    if (!frame) 
        MakeFrame();

    sprintf(wxio_buffer, "%*d", Width,  x);
    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(bool x)
{
    if (!frame) 
        MakeFrame();

    sprintf(wxio_buffer, "%c", (x) ? 'T' : 'F');
    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(long x)
{
    if (!frame) 
        MakeFrame();

    sprintf(wxio_buffer, "%*ld", Width, x);
    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(char x)
{
    if (!frame) 
        MakeFrame();
    sprintf(wxio_buffer, "%c", x);
    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(double x)
{
    if (!frame) 
        MakeFrame();

    switch (Represent)
    {
    case 'f':
        sprintf(wxio_buffer, "%*.*f", Width, Prec, x);
        break;

    case 'e':
        sprintf(wxio_buffer, "%*.*e", Width, Prec, x);
        break;
    }

    *frame->f << wxio_buffer;
    return *this;
}

gOutput &gWxOutput::operator<<(long double x)
{
  if (!frame) 
      MakeFrame();
    
  switch (Represent) {
  case 'f':
    sprintf(wxio_buffer, "%*.*Lf", Width, Prec, x);
    break;

  case 'e':
    sprintf(wxio_buffer, "%*.*Le", Width, Prec, x);
    break;
  }

  *frame->f << wxio_buffer;
  return *this;
}


gOutput &gWxOutput::operator<<(float x)
{
    if (!frame) 
        MakeFrame();

    switch (Represent)
    {
    case 'f':
        sprintf(wxio_buffer, "%*.*f", Width, Prec, x);
        break;

    case 'e':
        sprintf(wxio_buffer, "%*.*e", Width, Prec, x);
        break;
    }

    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(const char *x)
{
    if (!frame) 
        MakeFrame();

    sprintf(wxio_buffer, "%s", x);
    *frame->f << wxio_buffer;
    return *this;
}


gOutput &gWxOutput::operator<<(const void *x)
{
    if (!frame) 
        MakeFrame();

    sprintf(wxio_buffer, "%p", x);
    *frame->f << wxio_buffer;
    return *this;
}


bool gWxOutput::IsValid(void) const
{
    return true;
}


void gWxOutput::MakeFrame(void)
{
    assert(!frame && "Frame already exists");
    frame = new gWxIOFrame(this, (label) ? label : "WxIO");
#ifndef wx_msw
    frame->SetSize(200, 200); // does not resize automatically for some reason.
#endif
}

void gWxOutput::OnClose(void)
{
    if (frame)
    {
        // this is needed to avoid infinite recursion due to gWxIOFrame::OnClose and
        // allow the frame to be closed manually by the windows manager. NOT NECESSARY ?!
        //  gWxIOFrame *tmp_frame = frame;
        frame = 0;
        //  tmp_frame->Close();
    }
}

gWxOutput gwout("Default Output");
gWxOutput gwerr("Default Error");
gWxOutput *wout = &gwout;
gWxOutput *werr = &gwerr;
gOutput   &gout = gwout;
gOutput   &gerr = gwerr;

