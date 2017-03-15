/*
 * wtaccounts.h

 *
 *  Created on: Nov 7, 2016
 *      Author: judge
 */


#ifndef WTACCOUNTS_H
#define WTACCOUNTS_H

#include <Wt/WApplication>

#include "ui_wtaccounts.h"

//Below is the "Rendering HTML to PDF" code from the widget gallery, with the new container widget for a target.
namespace {
    void HPDF_STDCALL error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no,
               void *user_data) {
    fprintf(stderr, "libharu error: error_no=%04X, detail_no=%d\n",
        (unsigned int) error_no, (int) detail_no);
    }
}

class ReportResource : public Wt::WResource
{
public:
  ReportResource(Wt::WContainerWidget* target, Wt::WObject* parent = 0)
    : Wt::WResource(parent),
    _target(NULL)
  {
    suggestFileName("report.pdf");
    _target = target;
  }

  virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
  {
    response.setMimeType("application/pdf");

    HPDF_Doc pdf = HPDF_New(error_handler, 0);

    // Note: UTF-8 encoding (for TrueType fonts) is only available since libharu 2.3.0 !
    HPDF_UseUTFEncodings(pdf);

    renderReport(pdf);
    int ddsfd;
    HPDF_SaveToStream(pdf);
    unsigned int size = HPDF_GetStreamSize(pdf);
    HPDF_BYTE *buf = new HPDF_BYTE[size];
    HPDF_ReadFromStream (pdf, buf, &size);
    HPDF_Free(pdf);
    response.out().write((char*)buf, size);
    delete[] buf;
  }

private:

  Wt::WContainerWidget* _target;

  void renderReport(HPDF_Doc pdf)
  {
    std::stringstream ss;
    std::stringstream buffer;


    _target->htmlText(ss);
    std::string out = ss.str();
    std::string out_id = _target->id();
    std::string out_parent_id = _target->parent()->id();

    std::string STRING;
        std::ifstream infile("cssQ.txt");;

        if ( infile )
            {

                buffer << infile.rdbuf();

                infile.close();

                // operations on the buffer...
            }

       // infile.open

        std::string out3=buffer.str();
        size_t f =out3.find("201611-001979");
        out3.replace(f,std::string("This text is").length(), "Абдиль Жардибакев");

    std::ofstream out2("output.txt");
    out2 << out3;
    out2.close();



		//ss.str()
  renderPdf(Wt::WString::fromUTF8(out3), pdf);
   // renderPdf("<html><head></head><body><div style="text-align: center">centered content</div></body></html>", pdf);


  }

  void renderPdf(const Wt::WString& html, HPDF_Doc pdf)
  {

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
int d;
    Wt::Render::WPdfRenderer renderer(pdf, page);
   renderer.useStyleSheet("/resources/fgd.css");

    //renderer.st
   //renderer.setMargin(0);
  // renderer.setDpi(75);
    renderer.render(html);

  }
};





	// height = HPDF_Page_GetHeight (page);
	// width = HPDF_Page_GetWidth (page);

	 /* Print the title of the page (with positioning center). */
	//    def_font = HPDF_GetFont (pdf, "Helvetica", NULL);
	  //  HPDF_Page_SetFontAndSize (page, def_font, 24);

	   // tw = HPDF_Page_TextWidth (page, page_title);
	   // HPDF_Page_BeginText (page);
	//    HPDF_Page_TextOut (page, width , height - 50, page_title);
	 //   HPDF_Page_EndText (page);





class ReportResource2
{	std::string gow="";
public:

	ReportResource2(std::string d) : gow(d)
    {}
std::string ret(){return gow;}
};

class WtAccounts : public Wt::WApplication
{

public:
	WtAccounts(const Wt::WEnvironment& env);
	~WtAccounts();

	// declare slots here
	void data_load_user_account_tab();
	void create_user_tab_save_data();
	void edit_subscriber_load_data();
	// ***

	void superrefresh();

    void p_account_operation_CHECK(std::string operation_name);
    //data
    void p_account_operation_Data(std::string operation_name);
    //report
    void p_account_operation_Report(std::string operation_name);
    void p_account_operation_create_Report(std::string operation_name);
    //Serarch
    void Search_tree_Names(std::string operation_name);

	void subscriber_show_operation_tab(std::string operation_name);
	void subscriber_name_dialog(std::string operation_name);
	void subscriber_group_dialog(std::string operation_name);

	void subscriber_phone_number_dialog(std::string operation_name);
	void create_phone_number_dialog(std::string operation_name);
	void delete_phone_number_dialog(std::string operation_name);

	void subscriber_ip_address_dialog(std::string operation_name);
	void create_ip_address_dialog(std::string operation_name);
	void delete_ip_address_dialog(std::string operation_name);

	void subscriber_service_dialog(std::string operation_name);
	void add_service_dialog(std::string operation_name);
	void delete_service_dialog(std::string operation_name);

	void subscriber_tariff_plan_dialog(std::string operation_name);
	void subscriber_contact_dialog(std::string operation_name);
	void save_data_and_close_tab(std::string operation_name);

	// administrative functions
	// *************************************************************************
	void manage_services_dialog();
	void create_new_service_dialog();
	// *************************************************************************

	void subscriber_fullName_changed();


private:
	Ui_WtAccounts *ui;

};






#endif // WTACCOUNTS_H
