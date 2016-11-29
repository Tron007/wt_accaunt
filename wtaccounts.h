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
    _target->htmlText(ss);
    std::string out = ss.str();
    std::string out_id = _target->id();
    std::string out_parent_id = _target->parent()->id();

    renderPdf(Wt::WString::fromUTF8(ss.str()), pdf);
  }

  void renderPdf(const Wt::WString& html, HPDF_Doc pdf)
  {
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
int d;
    Wt::Render::WPdfRenderer renderer(pdf, page);
    renderer.setMargin(1);
    renderer.setDpi(96);
    renderer.render(html);
  }
};




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
