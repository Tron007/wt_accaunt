//============================================================================
// Name        : wt_accounts.cpp
// Author      : judge
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
 * wtaccounts.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: bigbang
 */

#include<string>
#include <set>
#include <mysql.h>
#include <mysql++.h>
#include <boost/lexical_cast.hpp>
#include "wtaccounts.h"





WtAccounts::WtAccounts(const Wt::WEnvironment& env) : Wt::WApplication(env), ui(new Ui_WtAccounts)
{
	ui->setupUi(root());
	//ui->menuitem_mi->clicked().connect(boost::bind(&WtAccounts::data_load_user_account_tab, this));
	//ui->user_group_button->clicked().connect(boost::bind(&WtAccounts::subscriber_group_dialog, this));
	//ui->user_full_name_button->clicked().connect(boost::bind(&WtAccounts::subscriber_name_dialog, this));
	//ui->tariff_plan_button->clicked().connect(boost::bind(&WtAccounts::subscriber_tariff_plan_dialog, this));
	//ui->ip_login_button->clicked().connect(boost::bind(&WtAccounts::subscriber_ip_address_dialog, this));
	//ui->menuitem_cp_mi->clicked().connect(boost::bind(&WtAccounts::create_ip_address_dialog, this));
	//ui->user_number_button->clicked().connect(boost::bind(&WtAccounts::subscriber_phone_number_dialog, this));
	//ui->nv_menu_item2_mi->clicked().connect(boost::bind(&WtAccounts::edit_subscriber_load_data, this)); // ***
	//ui->save_close_button->clicked().connect(boost::bind(&WtAccounts::create_user_tab_save_data, this));
	ui->main_tabs->setCurrentIndex(0);
	//WtAccounts::data_load_user_account_tab();
	WtAccounts::subscriber_show_operation_tab("view");


	// new regrouping functions
	//ui->nv_menu_item1_mi->clicked().connect(boost::bind(&WtAccounts::subscriber_show_operation_tab, this, "create"));
	//ui->nv_menu_item2_mi->clicked().connect(boost::bind(&WtAccounts::subscriber_show_operation_tab, this, "edit"));

	//Drop out M (Ama data,Netflow data)(make and configure report)
	ui->p_account_operation_split_button_popup->itemSelected().connect(boost::bind(&WtAccounts::p_account_operation_CHECK, this, "PopUp"));

    //Search
	//ui->user_search_edit->changed().connect(boost::bind(&WtAccounts::Search_tree_Names, this, "Search"));
	ui->user_search_button->clicked().connect(boost::bind(&WtAccounts::Search_tree_Names, this, "Search"));

	///

	ui->user_full_name_button->clicked().connect(boost::bind(&WtAccounts::subscriber_name_dialog, this, "create"));
	ui->user_full_name_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_name_dialog, this, "edit"));
	ui->user_group_button->clicked().connect(boost::bind(&WtAccounts::subscriber_group_dialog, this, "create"));
	ui->user_group_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_group_dialog, this, "edit"));
	ui->user_number_button->clicked().connect(boost::bind(&WtAccounts::subscriber_phone_number_dialog, this, "create"));
	ui->user_number_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_phone_number_dialog, this, "edit"));
	ui->ip_login_button->clicked().connect(boost::bind(&WtAccounts::subscriber_ip_address_dialog, this, "create"));
	ui->ip_login_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_ip_address_dialog, this, "edit"));
	ui->tariff_plan_button->clicked().connect(boost::bind(&WtAccounts::subscriber_tariff_plan_dialog, this, "create"));
	ui->tariff_plan_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_tariff_plan_dialog, this, "edit"));
	ui->user_address_button->clicked().connect(boost::bind(&WtAccounts::subscriber_contact_dialog, this, "create"));
	ui->user_address_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_contact_dialog, this, "edit"));
	ui->save_close_button->clicked().connect(boost::bind(&WtAccounts::save_data_and_close_tab, this, "create"));
	ui->save_close_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::save_data_and_close_tab, this, "edit"));
	ui->service_button->clicked().connect(boost::bind(&WtAccounts::subscriber_service_dialog, this, "create"));
	ui->service_button_edit_user_tab->clicked().connect(boost::bind(&WtAccounts::subscriber_service_dialog, this, "edit"));
	//ui->user_treeTable->tree()->itemSelectionChanged().connect(boost::bind(&WtAccounts::subscriber_fullName_changed, this));


	ui->nv_pp_item1->clicked().connect(boost::bind(&WtAccounts::subscriber_show_operation_tab, this, "view"));
	ui->nv_pp_item2->clicked().connect(boost::bind(&WtAccounts::subscriber_show_operation_tab, this, "create"));
	ui->nv_pp_item3->clicked().connect(boost::bind(&WtAccounts::subscriber_show_operation_tab, this, "edit"));
	ui->nv_pp_item4->clicked().connect(boost::bind(&WtAccounts::manage_services_dialog, this));
}

WtAccounts::~WtAccounts()
{
	delete ui;
}


// mysql variables for connection, store result
MYSQL *conn, mysql;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_RES *add_res;
MYSQL_ROW add_row;

// mysql connections settings
const char *server="52.8.83.48";
const char *user="007";
const char *password="007";
const char *database="account_database";

int query_state;
std::string mysql_query_str = "";




// table variable for update data in table
Wt::WTable *ip_address_table;
Wt::WTable *phone_number_table;
Wt::WTable *subscriber_connected_table; // subscriber connected services table
Wt::WTable *service_table;

// create subscriber varibles
// subscriber group info variable
std::string group_name = "";

// subscriber info variables
std::string subscriber_id = "";
std::string group_id = "";
std::string name = "";
std::string surname = "";
std::string patronymic = "";
std::string full_name = "";
std::string subscriber_type = "";
std::string account_number = "";
std::string user_login = "";
std::string user_password = "";
std::string date_since = "";
std::string date_by = "";
std::string tariff_plan  = "";
std::string user_email = "";

// subscriber contacts data variables
std::string city = "";
std::string district = "";
std::string street = "";
std::string housing = "";
std::string house_floor = "";
std::string house = "";
std::string apartment = "";
std::string index_number = "";
std::string contact_numbers = "";

// subscriber requisites data variables
std::string personal_code = "";
std::string organization_code = "";
std::string tax_code = "";
std::string account = "";
std::string bank_code = "";
std::string agreement = "";
std::string agreement_date = "";

// subscriber information fields data variables
std::string description = "";
std::string information = "";



// edit subscriber variables
// subscriber group info variable
std::string group_name_edit_mode = "";
std::string new_group_name_edit_mode = ""; // used for edit tab when subscriber group changed name saved in this variable

// subscriber info variables
std::string subscriber_id_edit_mode = "";
std::string group_id_edit_mode = "";
std::string name_edit_mode = "";
std::string surname_edit_mode = "";
std::string patronymic_edit_mode = "";
std::string full_name_edit_mode = "";
std::string subscriber_type_edit_mode = "";
std::string account_number_edit_mode = "";
std::string user_login_edit_mode = "";
std::string user_password_edit_mode = "";
std::string date_since_edit_mode = "";
std::string date_by_edit_mode = "";
std::string tariff_plan_edit_mode  = "";
std::string user_email_edit_mode = "";

// subscriber contacts data variables
std::string city_edit_mode = "";
std::string district_edit_mode = "";
std::string street_edit_mode = "";
std::string housing_edit_mode = "";
std::string house_floor_edit_mode = "";
std::string house_edit_mode = "";
std::string apartment_edit_mode = "";
std::string index_number_edit_mode = "";
std::string contact_numbers_edit_mode = "";

// subscriber requisites data variables
std::string personal_code_edit_mode = "";
std::string organization_code_edit_mode = "";
std::string tax_code_edit_mode = "";
std::string account_edit_mode = "";
std::string bank_code_edit_mode = "";
std::string agreement_edit_mode = "";
std::string agreement_date_edit_mode = "";

// subscriber information fields data variables
std::string description_edit_mode = "";
std::string information_edit_mode = "";

int dor=0;


//PDF func
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
	 std::string nate;
	 std::string nate2;
	 std::string nate3;
	 int number;
/*
  ReportResource(Wt::WContainerWidget* target, Wt::WObject* parent = 0)
    : Wt::WResource(parent),
    _target(NULL)
  {
    suggestFileName("report.pdf");
    _target = target;
  }
*/
   ReportResource(Wt::WContainerWidget* target,int d=0)
	     :number(d)
	   {
	     suggestFileName("report.pdf");
	     _target = target;
	   }

   ReportResource(Wt::WContainerWidget* target,std::string name="",std::string text="")
        :nate(name),nate2(text)
      {
  	std::string Obriv=" ";
  	std::size_t pos_for_Obriv;

  	//for addition name in tab
  	pos_for_Obriv = nate.find(' ');
  	Obriv += nate.substr(0,pos_for_Obriv);


        suggestFileName(Wt::WString::fromUTF8(Obriv)+".pdf");
        _target = target;
      }

/*
  ReportResource(Wt::WContainerWidget* target,std::string name="",std::string Number="",std::string rewq="")
      :nate(name),nate2(Number),nate3(rewq)
    {
	std::string Obriv=" ";
	std::size_t pos_for_Obriv;

	//for addition name in tab
	pos_for_Obriv = nate.find(' ');
	Obriv += nate.substr(0,pos_for_Obriv);


      suggestFileName(Wt::WString::fromUTF8(Obriv)+".pdf");
      _target = target;
    }
*/
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

    std::string STRING;






		//ss.str()
  renderPdf(Wt::WString::fromUTF8(nate2), pdf);



  }

  void renderPdf(const Wt::WString& html, HPDF_Doc pdf)
  {

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    int d;
    Wt::Render::WPdfRenderer renderer(pdf, page);
  // renderer.useStyleSheet("/resources/fgd.css");

    const char *fontname;
    HPDF_Font font;

 //   fontname = HPDF_LoadTTFontFromFile(pdf, "FreeSans.ttf", HPDF_TRUE);
 //   font = HPDF_GetFont(pdf, fontname, "UTF-8");
  //  HPDF_Page_SetFontAndSize(page, font, 20.0);

    //renderer.st
   //renderer.setMargin(0);
  // renderer.setDpi(70);
   renderer.render(html);

  }
};

//PDF func END



// zone fixed, clear function
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//Search_tree_Names
extern void WtAccounts::Search_tree_Names(std::string operation_name) {


	std::string ResultOf_Search="";
	ResultOf_Search=ui->user_search_edit->text().toUTF8();

	if (ResultOf_Search=="" || ResultOf_Search==" "){subscriber_show_operation_tab("view");
	}
	else
	{
		mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");


	ui->user_treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
	ui->user_treeTable->tree()->setSelectionMode(Wt::SingleSelection);
	//ui->user_treeTable->addColumn(Wt::WString::fromUTF8("Абонентов"), 100);


    ui->user_tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8(ResultOf_Search));
	ui->user_treeTable->setTreeRoot(ui->user_tree_root, Wt::WString::fromUTF8("Поиск"));

    Wt::WTreeTableNode *tree_node;

					std::string mysql_get_subscriber_fullName = " SELECT full_name  FROM account_database.subscriber   WHERE locate('"+ResultOf_Search+"',full_name)>0 UNION ALL "
							"SELECT full_name  FROM account_database.subscriber AS a INNER JOIN account_database.phone_numbers AS p ON a.subscriber_id=p.subscriber_id  WHERE locate('"+ResultOf_Search+"', p.number)>0 UNION ALL "
									" SELECT full_name  FROM account_database.subscriber AS a INNER JOIN account_database.ip_addresses AS i ON a.subscriber_id=i.subscriber_id  WHERE locate('"+ResultOf_Search+"', i.ip_address)>0";
					std::cout<<"Trying to Search "<<ResultOf_Search<<std::endl<<std::endl;

						query_state=mysql_query(conn, mysql_get_subscriber_fullName.c_str());

						if(query_state!=0)
						{
						   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
						}

						add_res=mysql_store_result(conn);
						while ((add_row=mysql_fetch_row(add_res))!=NULL)
						{
						tree_node = new Wt::WTreeTableNode(Wt::WString::fromUTF8(add_row[0]), 0, ui->user_tree_root);
			            }

					mysql_free_result(add_res);
					mysql_close(conn);

					ui->user_tree_root->expand();
	}



}
//End of Search



extern void WtAccounts::p_account_operation_Data(std::string operation_name)
{
//get name of M that pressed
std::string ResultOfoperationmeny="";
ResultOfoperationmeny=ui->p_account_operation_split_button_popup->result()->text().toUTF8();

//First check(if just table data) creat table 1st then perfom another function

	Wt::WMessageBox *messageBox;

	//check year
	std::string ResulYearCombo="";
	ResulYearCombo=ui->year_combo_box->currentText().toUTF8();
	//check month
	int ResulMonthCombo_index;
	ResulMonthCombo_index=ui->month_combo_box->currentIndex();ResulMonthCombo_index++;
	std::string ResulMonthCombo_index_string=std::to_string(ResulMonthCombo_index);



	std::string changedSubscriberName = "";
	Wt::WTreeNode *selected_node; // operator* returns contents of an interator
	std::set<Wt::WTreeNode* > highlightedRows = ui->user_treeTable->tree()->selectedNodes();


			Wt::WContainerWidget * CHECK_pop_tab_Temp = new Wt::WContainerWidget(ui->container_cp);
			Wt::WMenuItem * CHECK_pop_tab_mi_temp = ui->main_tabs->addTab(CHECK_pop_tab_Temp, Wt::WString::fromUTF8(""));;
			Wt::WTable *CHECK_user_treeTablef;
			CHECK_pop_tab_mi_temp->setCloseable(true);
			CHECK_pop_tab_mi_temp->enable();

			Wt::WContainerWidget *service_table_container = new Wt::WContainerWidget(CHECK_pop_tab_Temp);
			service_table_container->setHtmlTagName("div");
			service_table_container->setHeight(Wt::WLength("800px"));

			service_table_container->setOverflow(Wt::WContainerWidget::Overflow::OverflowAuto, Wt::Orientation::Vertical);

			CHECK_user_treeTablef = new Wt::WTable(service_table_container);
			CHECK_user_treeTablef->setId("CHECK_user_treeTable");
			CHECK_user_treeTablef->setStyleClass(Wt::WString::fromUTF8("table table-bordered table-striped table-hover table-condensed"));
			CHECK_user_treeTablef->setInline(0);
			CHECK_user_treeTablef->setHeaderCount(1, Wt::Orientation::Horizontal);
			CHECK_user_treeTablef->setHeaderCount(1, Wt::Orientation::Vertical);


			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
							     					{
							     					  selected_node = *i;
							     					}
				                      changedSubscriberName = selected_node->label()->text().toUTF8();


		  //for addition name in tab
		std::string Obriv=" ";
		 std::size_t pos_for_Obriv;
		 /*
		for(std::string::iterator it = changedSubscriberName.begin(); it != changedSubscriberName.end(); ++it) {


		}*/

		//for addition name in tab
		pos_for_Obriv = changedSubscriberName.find(' ');
		Obriv += changedSubscriberName.substr(0,pos_for_Obriv);
		/*for (auto single_char:changedSubscriberName){
			if (single_char!=' ') Obriv+=single_char;
			else break;
		 }*/

		CHECK_pop_tab_mi_temp->setText(Wt::WString::fromUTF8(ResultOfoperationmeny+Obriv));

					ui->main_tabs->setCurrentIndex(ui->main_tabs->indexOf(CHECK_pop_tab_Temp));

					CHECK_user_treeTablef->clear();
					CHECK_user_treeTablef->refresh();


					CHECK_user_treeTablef->setHeaderCount(1);

/*
					//PDF
				Wt::WResource *pdf = new ReportResource(CHECK_pop_tab_Temp);

										Wt::WPushButton *button2 = new Wt::WPushButton("Create pdf",CHECK_pop_tab_Temp);
										//Wt::WPushButton *button3 = new Wt::WPushButton("wwwwwwwwwwww",ui->container_cp);
										button2->setLink(pdf);
					//PDF
*/


					if (Wt::WString::fromUTF8("Телефонный трафик")==Wt::WString::fromUTF8(ResultOfoperationmeny)){

						CHECK_user_treeTablef->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
						CHECK_user_treeTablef->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Запись")));
						CHECK_user_treeTablef->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тип")));
						CHECK_user_treeTablef->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер А")));
						CHECK_user_treeTablef->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер Б")));
						CHECK_user_treeTablef->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Дата")));
					    CHECK_user_treeTablef->elementAt(0, 6)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Время")));
						CHECK_user_treeTablef->elementAt(0, 7)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Код")));
					    CHECK_user_treeTablef->elementAt(0, 8)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Абонент")));
						CHECK_user_treeTablef->elementAt(0, 9)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Откуда")));
						CHECK_user_treeTablef->elementAt(0, 10)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Город")));
			     		CHECK_user_treeTablef->elementAt(0, 11)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Категория")));
						CHECK_user_treeTablef->elementAt(0, 12)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Секунд вызова")));
			     		CHECK_user_treeTablef->elementAt(0, 13)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Стоимость")));


			       highlightedRows.clear();


mysql_init(&mysql);
conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
mysql_query(&mysql,"SET NAMES 'UTF8'");

			     	if(conn==NULL){
			     		std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;}


			     	std::string mysql_subscriber_all_table_data = "SELECT c.*,full_name FROM account_database.subscriber "
			     			" AS a INNER JOIN account_database.phone_numbers AS b INNER JOIN account_database.ama_data AS "
			     			"c ON a.subscriber_id=b.subscriber_id WHERE full_name='"+changedSubscriberName+"' "
			     					"AND (b.number=c.numberB OR b.number=c.numberA) "
			     					"AND year(c.start_date)=year(str_to_date('"+ResulYearCombo+"','%Y')) AND month(c.start_date)=month(str_to_date('"+ResulMonthCombo_index_string+"','%m')) ";

			     				int row_number = 0;
			     				query_state=mysql_query(conn, mysql_subscriber_all_table_data.c_str());
			     				if(query_state!=0)
			     				{
			     				   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			     				}
			     				std::string TypeOfcall="";
			     				res=mysql_store_result(conn);
			     				while((row=mysql_fetch_row(res))!=NULL)
			     				{row_number++;

			     				if (Wt::WString::fromUTF8(row[11])=="common") TypeOfcall="Городской";
			     				if (Wt::WString::fromUTF8(row[11])=="outgoing mobile") TypeOfcall="Входяший";
			     				if (Wt::WString::fromUTF8(row[11])=="international") TypeOfcall="Междугородный";

			     				new Wt::WText(Wt::WString::fromUTF8("Учет телефонных звонков"), CHECK_user_treeTablef->elementAt(row_number, 0));
			     				new Wt::WText(Wt::WString::fromUTF8("Обычный"), CHECK_user_treeTablef->elementAt(row_number, 1));
			     				new Wt::WText(Wt::WString::fromUTF8(row[1]), CHECK_user_treeTablef->elementAt(row_number, 3));
			     				new Wt::WText(Wt::WString::fromUTF8(row[2]), CHECK_user_treeTablef->elementAt(row_number, 4));
			     				new Wt::WText(Wt::WString::fromUTF8(row[3]), CHECK_user_treeTablef->elementAt(row_number, 5));
			     				new Wt::WText(Wt::WString::fromUTF8(row[4]), CHECK_user_treeTablef->elementAt(row_number, 6));
			     				new Wt::WText(Wt::WString::fromUTF8(changedSubscriberName), CHECK_user_treeTablef->elementAt(row_number, 8));
			     				new Wt::WText(Wt::WString::fromUTF8(TypeOfcall), CHECK_user_treeTablef->elementAt(row_number, 2));
			     				}
			     				CHECK_user_treeTablef->refresh();
			     				mysql_free_result(res);
			     				mysql_close(conn);
					}


					if (Wt::WString::fromUTF8("Netflow трафик")==Wt::WString::fromUTF8(ResultOfoperationmeny)){


						CHECK_user_treeTablef->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
						CHECK_user_treeTablef->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Источник IP")));
						CHECK_user_treeTablef->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Порт Источника")));
						CHECK_user_treeTablef->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Цель IP")));
						CHECK_user_treeTablef->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Порт назначения")));
						CHECK_user_treeTablef->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Протокол")));
						CHECK_user_treeTablef->elementAt(0, 6)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Дата/Время")));
						CHECK_user_treeTablef->elementAt(0, 7)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Байт передано")));
						CHECK_user_treeTablef->elementAt(0, 8)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Абонент")));




			       highlightedRows.clear();


mysql_init(&mysql);
conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
mysql_query(&mysql,"SET NAMES 'UTF8'");

			     	if(conn==NULL){
			     		std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;}
			     	if(conn==NULL){
			     		std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;}



			     	std::string mysql_subscriber_all_table_data = "SELECT c.receive_date,c.source_address,Sum(c.total_bytes),full_name "
			     			"FROM account_database.subscriber AS a INNER JOIN account_database.ip_addresses  AS b "
			     			"INNER JOIN account_database.netflow_data AS c ON a.subscriber_id=b.subscriber_id  "
			     			"WHERE b.ip_address=c.source_address AND full_name='"+changedSubscriberName+"' "
			     			"AND year(c.receive_date)=year(str_to_date('"+ResulYearCombo+"','%Y')) AND month(c.receive_date)=month(str_to_date('"+ResulMonthCombo_index_string+"','%m')) "
			     					"group by DATE_FORMAT(c.receive_date, '%d')";



			     				int row_number = 0;
			     				query_state=mysql_query(conn, mysql_subscriber_all_table_data.c_str());
			     				if(query_state!=0)
			     				{
			     				   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			     				}
			     				std::string TypeOfcall="";
			     				res=mysql_store_result(conn);
			     				while((row=mysql_fetch_row(res))!=NULL)
			     				{row_number++;


			     				new Wt::WText(Wt::WString::fromUTF8("Netflow трафик"), CHECK_user_treeTablef->elementAt(row_number, 0));
			     				new Wt::WText(Wt::WString::fromUTF8("0.0.0.0"), CHECK_user_treeTablef->elementAt(row_number, 1));
			     				new Wt::WText(Wt::WString::fromUTF8("0"), CHECK_user_treeTablef->elementAt(row_number, 2));
			     				new Wt::WText(Wt::WString::fromUTF8(row[1]), CHECK_user_treeTablef->elementAt(row_number, 3));
			     				new Wt::WText(Wt::WString::fromUTF8("0"), CHECK_user_treeTablef->elementAt(row_number, 4));
			     				new Wt::WText(Wt::WString::fromUTF8("0"), CHECK_user_treeTablef->elementAt(row_number, 5));
			     				new Wt::WText(Wt::WString::fromUTF8(row[0]), CHECK_user_treeTablef->elementAt(row_number, 6));
			     				new Wt::WText(Wt::WString::fromUTF8(row[2]), CHECK_user_treeTablef->elementAt(row_number, 7));
			     				new Wt::WText(Wt::WString::fromUTF8(row[3]), CHECK_user_treeTablef->elementAt(row_number, 8));

			     				}
			     				CHECK_user_treeTablef->refresh();
			     				mysql_free_result(res);
			     				mysql_close(conn);









				}}
//end of func of showing data (netflow calls)


//func to make creat report
extern void WtAccounts::p_account_operation_create_Report(std::string operation_name)
{

	Wt::WMessageBox *messageBox;

		//check year
		std::string ResulYearCombo="";
		ResulYearCombo=ui->year_combo_box->currentText().toUTF8();
		//check month
		int ResulMonthCombo_index;
		ResulMonthCombo_index=ui->month_combo_box->currentIndex();ResulMonthCombo_index++;
		std::string ResulMonthCombo_index_string=std::to_string(ResulMonthCombo_index);

		std::string ResultOfoperationmeny="";
		ResultOfoperationmeny=ui->p_account_operation_split_button_popup->result()->text().toUTF8();


		std::string changedSubscriberName = "";
		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = ui->user_treeTable->tree()->selectedNodes();

		//get name that selected
		for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
									     					{
									     					  selected_node = *i;
									     					}
			changedSubscriberName = selected_node->label()->text().toUTF8();


			//creat tab
				Wt::WContainerWidget * CHECK_pop_tab_Temp = new Wt::WContainerWidget(ui->container_cp);
				Wt::WMenuItem * CHECK_pop_tab_mi_temp = ui->main_tabs->addTab(CHECK_pop_tab_Temp, Wt::WString::fromUTF8(""));;
				Wt::WTable *CHECK_user_treeTablef;
				CHECK_pop_tab_mi_temp->setCloseable(true);
				CHECK_pop_tab_mi_temp->enable();

			 CHECK_pop_tab_mi_temp->setText(Wt::WString::fromUTF8(ResultOfoperationmeny));

			ui->main_tabs->setCurrentIndex(ui->main_tabs->indexOf(CHECK_pop_tab_Temp));

			Wt::WContainerWidget *service_table_container = new Wt::WContainerWidget(CHECK_pop_tab_Temp);



			////

			//connect to data base
			mysql_init(&mysql);
			conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
			mysql_query(&mysql,"SET NAMES 'UTF8'");

			if(conn==NULL){
			std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;}



			std::string mysql_Data_for_report = "SELECT * FROM  account_database.requisites WHERE subscriber_id IN "
					"(SELECT subscriber_id FROM  account_database.subscriber WHERE full_name = '"+changedSubscriberName+"')";

			query_state=mysql_query(conn, mysql_Data_for_report.c_str());
			// set variables and form elements with data from mysql tables

					if(query_state!=0)
								   {
								 std::cout<<mysql_error(conn)<<std::endl<<std::endl;
								   }
								   res=mysql_store_result(conn);
								    std::cout<<"MySQL Values in the amaDB Table. Report"<<std::endl<<std::endl;
								    std::string Presonal_code="";
								    std::string agreement="";
								    if((row=mysql_fetch_row(res))!=NULL) {





						     				Presonal_code=row[2];
						     				agreement=row[7];}

						     				mysql_free_result(res);






			////
			//Use difrent css
		// Add an external style sheet to the application.
		//Wt::WApplication::instance()->useStyleSheet("/resources/Report.css");
		//Wt::WApplication::instance()->useStyleSheet("/resources/fgd.css");
		// The style sheet should be applied to this container only.
		// The class .CSS-example is used as selector.
		//service_table_container->setStyleClass("CSS-example");

			//Wt::WResource *pdf = new ReportResource(service_table_container,5);

				std::ifstream infile("cssQ.txt");
				std::string out3;
				std::stringstream buffer;

		if ( infile )
		{
			buffer << infile.rdbuf();
			infile.close();
			out3=buffer.str();
			// operations on the buffer...
		}


          // infile.open
		size_t f =out3.find("Name_w");
		out3.replace(f,std::string("Name_w").length(), changedSubscriberName);
	    f =out3.find("personal_code");
		out3.replace(f,std::string("personal_code").length(), Presonal_code);

		f =out3.find("req_code");
		out3.replace(f,std::string("req_code").length(), agreement);
	    f =out3.find("req_code");
		out3.replace(f,std::string("req_code").length(), agreement);


		f =out3.find("year_month");
		out3.replace(f,std::string("year_month").length(), ResulYearCombo+ResulMonthCombo_index_string);

		f =out3.find("month_w");
		out3.replace(f,std::string("month_w").length(), ResulMonthCombo_index_string);

		f =out3.find("year_n");
		out3.replace(f,std::string("year_n").length(), ResulYearCombo);


        int numberforskip=244;//244 = FULL text size  between <!--ServiceStart--> to <!--ServiceEnd-->
		std::string Service = out3.substr(out3.find("<!--ServiceStart-->")+std::string("<!--ServiceStart-->").length(),numberforskip);


		mysql_Data_for_report = "SELECT description,quantity FROM account_database.subscriber_transaction WHERE subscriber_id"
				" IN  (SELECT subscriber_id FROM  account_database.subscriber WHERE full_name = '"+changedSubscriberName+"')";
				//		"AND  month(transaction_date)=month(str_to_date('"+ResulMonthCombo_index_string+"','%m')) AND year(transaction_date)=year(str_to_date('"+ResulYearCombo+"','%Y'))";
				    int row_number = 0;

				    query_state=mysql_query(conn, mysql_Data_for_report.c_str());
				    // set variables and form elements with data from mysql tables

				    if(query_state!=0)
				    		{
				    		std::cout<<mysql_error(conn)<<std::endl<<std::endl;
				    		}

				    res=mysql_store_result(conn);
				    numberforskip=0;

				    std::cout<<"MySQL Values in the amaDB Table. Report2"<<std::endl<<std::endl;

				    if((row=mysql_fetch_row(res))!=NULL) {


				   f = out3.find("ServiceName");
				   out3.replace(f,std::string("ServiceName").length(), row[0]);

				   f = out3.find("ServicePrice");
				   out3.replace(f,std::string("ServicePrice").length(), row[1]);

				   std::cout<<"MySQL Values in the amaDB Table. "<<row[0]<<std::endl<<std::endl;



				    //if more than 1
					while((row=mysql_fetch_row(res))!=NULL)
					{row_number++;


							     	std::string TempLength1=row[0];
							     	std::string TempLength2=row[1];
							     	TempLength1+=TempLength2;
							     	std::string ServiceTemp=Service;

							     	f =ServiceTemp.find("ServiceName");
							     	ServiceTemp.replace(f,std::string("ServiceName").length(), row[0]);

							     	f =ServiceTemp.find("ServicePrice");
							     	ServiceTemp.replace(f,std::string("ServicePrice").length(), row[1]);

					out3.insert(out3.find("<!--ServiceEnd-->")+std::string("<!--ServiceEnd-->").length()+numberforskip,ServiceTemp);
					numberforskip+=220+TempLength1.length()+1;//220 is size of pure HTML wiout any text + text that we added to skip forvard
							     				}

							      mysql_free_result(res);
								  mysql_close(conn);

 std::string Copytext = out3.substr(out3.find("<!--CopyStart-->")+std::string("<!--CopyStart-->").length(),out3.find("<!--CopyEnd-->")-(out3.find("<!--CopyStart-->")+std::string("<!--CopyStart-->").length()));



		out3+=Copytext+"</div>";

		std::ofstream out2("output.txt");
		out2 << out3;
		out2.close();






			 Wt::WResource *pdf = new ReportResource(service_table_container,changedSubscriberName,out3);

			  Wt::WPushButton *button2 = new Wt::WPushButton("Create pdf",service_table_container);


			  //link to rendered  file
			 button2->setLink(pdf);

				    }
				    else {
						messageBox = new Wt::WMessageBox(Wt::WString::fromUTF8("Ошибка"), Wt::WString::fromUTF8("Не выбран абонент"), Wt::Information, Wt::Yes | Wt::No);
						messageBox->buttonClicked().connect(std::bind([=] () {
						delete messageBox;
						}));

						messageBox->show();
					}



}


//func to make report
extern void WtAccounts::p_account_operation_Report(std::string operation_name)
{

//NOT READY (func for another buttons)!!!!!
// show dialog window where you manage reports (create)



	    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Отчеты"));

	    dialog->resize(500, 300);

Wt::WPushButton *save_exit_button = new Wt::WPushButton(Wt::WString::fromUTF8("Квитанция для физ."), dialog->contents());
	        //save_exit_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

Wt::WPushButton *add_service_button = new Wt::WPushButton(Wt::WString::fromUTF8("Редактирование квитанции"), dialog->contents());
	        //add_service_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

Wt::WPushButton *add2_service_button = new Wt::WPushButton(Wt::WString::fromUTF8("Расшифровка город и межгород"), dialog->contents());
	              //  add_service_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

	                dialog->show();

	                Wt::WPushButton *cancel = new Wt::WPushButton(Wt::WString::fromUTF8("Закрыть"), dialog->footer());
	                    dialog->rejectWhenEscapePressed();


	                    // Accept the dialog

	                    save_exit_button->clicked().connect(std::bind([=] () {
	                    	WtAccounts::p_account_operation_create_Report("edit");  dor=1;dialog->accept();delete dialog;
	                    }));

	                    add_service_button->clicked().connect(std::bind([=] () {
	                    	  WtAccounts::subscriber_show_operation_tab("view");    dor=2;dialog->accept();delete dialog;
                         }));

	                    add2_service_button->clicked().connect(std::bind([=] () {
	                    	WtAccounts::subscriber_show_operation_tab("create");    dor=3;dialog->accept();delete dialog;
	                    }));

	                    // Reject the dialog
	                    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


	                    // Process the dialog result.
	                    dialog->finished().connect(std::bind([=] () {
	                	if (dor==1)      {}
	                	else if (dor==2) {}
	                	else if (dor==3) {}
	                	else if (dor==0) {}
	                	dor==0;
	                    }));

}

// function for operation on check
extern void WtAccounts::p_account_operation_CHECK(std::string operation_name)
{
	Wt::WMessageBox *messageBox;

//get name of M that pressed
std::string ResultOfoperationmeny="";
ResultOfoperationmeny=ui->p_account_operation_split_button_popup->result()->text().toUTF8();

std::set<Wt::WTreeNode* > highlightedRows = ui->user_treeTable->tree()->selectedNodes();
if (!highlightedRows.empty())
				{

if (Wt::WString::fromUTF8("Телефонный трафик")==Wt::WString::fromUTF8(ResultOfoperationmeny) || Wt::WString::fromUTF8("Netflow трафик")==Wt::WString::fromUTF8(ResultOfoperationmeny)){
	p_account_operation_Data("Телефонный трафик");
}
if (Wt::WString::fromUTF8("Новый отчет")==Wt::WString::fromUTF8(ResultOfoperationmeny)){
	p_account_operation_Report("Новый отчет");
}
				}
else
				{
					messageBox = new Wt::WMessageBox(Wt::WString::fromUTF8("Ошибка"), Wt::WString::fromUTF8("Не выбран абонент"), Wt::Information, Wt::Yes | Wt::No);
					messageBox->buttonClicked().connect(std::bind([=] () {
					delete messageBox;
					}));

					messageBox->show();
				}}


///

// function show required tab, depend on operation_name
extern void WtAccounts::subscriber_show_operation_tab(std::string operation_name)
{

	if (operation_name == "view") // *************************************************************************
	{
		//ui->user_account_tab_mi->setText("Adilbek Cool");
		ui->user_account_tab_mi->setHidden(false);
		//ui->main_tabs->setTabEnabled(0, 1);
		ui->main_tabs->setCurrentIndex(0);





		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");


		ui->user_treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
		ui->user_treeTable->tree()->setSelectionMode(Wt::SingleSelection);
		//ui->user_treeTable->addColumn(Wt::WString::fromUTF8("Абонентов"), 100);


	    ui->user_tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("ТОО"));
		ui->user_treeTable->setTreeRoot(ui->user_tree_root, Wt::WString::fromUTF8("Группа"));

	    Wt::WTreeTableNode *tree_node;

	// add node form mysql table
		query_state=mysql_query(conn, "select * from subscriber_group");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		std::string mysql_get_subscriber_fullName = "";
		std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
		while((row=mysql_fetch_row(res))!=NULL)
		{
			ui->user_tree_group  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[1]), 0, ui->user_tree_root);
			ui->user_tree_group->setSelectable(false);

			mysql_get_subscriber_fullName = "select full_name from subscriber where subscriber.group_id = '"+boost::lexical_cast<std::string>(row[0])+"'";
			query_state=mysql_query(conn, mysql_get_subscriber_fullName.c_str());
			if(query_state!=0)
			{
			   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			}
			add_res=mysql_store_result(conn);
			while((add_row=mysql_fetch_row(add_res))!=NULL)
			{
				//ui->user_tree_group ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(add_row[0])));
				tree_node = new Wt::WTreeTableNode(Wt::WString::fromUTF8(add_row[0]), 0, ui->user_tree_group);
			}
		}
		//ui->user_tree_group->clearSides();
		mysql_free_result(res);
		mysql_free_result(add_res);
		mysql_close(conn);

		ui->user_tree_root->expand();



/*
		ui->user_treeTable->tree()->setSelectionMode(Wt::ExtendedSelection);
		ui->user_treeTable->addColumn(Wt::WString::fromUTF8("Номера"), 100);
		ui->user_treeTable->addColumn(Wt::WString::fromUTF8("Все номера"), 125);
		ui->user_treeTable->addColumn(Wt::WString::fromUTF8("Дата подк."), 125);
		ui->user_treeTable->addColumn(Wt::WString::fromUTF8("Дата откл."), 125);

		ui->user_tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("ТОО"));
		ui->user_treeTable->setTreeRoot(ui->user_tree_root, Wt::WString::fromUTF8("Абонент"));






		ui->user_tree_group = new Wt::WTreeTableNode(Wt::WString::fromUTF8("SIP Telecom"), 0, ui->user_tree_root);
		addNode(ui->user_tree_group, Wt::WString::fromUTF8("Ешимбетов Адильбек"), "423423", "127.0.0.1", "20.07/2016", "");
		addNode(ui->user_tree_group, Wt::WString::fromUTF8("Волошин Олег"), "534534", "190.165.257.130", "20.07.2016", "");
		addNode(ui->user_tree_group, Wt::WString::fromUTF8("Адам Ламберт"), "344556", "10.33.0.123", "20.07.2016", "");

		ui->user_tree_group = new Wt::WTreeTableNode("Digital System Servis", 0, ui->user_tree_root);


		ui->user_tree_group = new Wt::WTreeTableNode("KazTransCom", 0, ui->user_tree_root);


		ui->user_tree_root->expand();
*/



	// tables
	// ************************************************************************************************

		// total month credit table

		ui->total_month_credit_table->clear();
		ui->total_month_credit_table->refresh();

		ui->total_month_credit_table->setHeaderCount(1);

		ui->total_month_credit_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Услуга")));
		ui->total_month_credit_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Код")));
		ui->total_month_credit_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Количество")));
		ui->total_month_credit_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Сумма")));
		ui->total_month_credit_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер/IP")));
		ui->total_month_credit_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Использование")));
		ui->total_month_credit_table->elementAt(0, 6)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

	    new Wt::WText(Wt::WString::fromUTF8("Абонентская плата за телефон"), ui->total_month_credit_table->elementAt(1, 0));
	    new Wt::WText(Wt::WString::fromUTF8("1.1"), ui->total_month_credit_table->elementAt(1, 1));
	    new Wt::WText(Wt::WString::fromUTF8("1"), ui->total_month_credit_table->elementAt(1, 2));
	    new Wt::WText(Wt::WString::fromUTF8("619.3500"), ui->total_month_credit_table->elementAt(1, 3));
	    new Wt::WText(Wt::WString::fromUTF8(""), ui->total_month_credit_table->elementAt(1, 4));
	    new Wt::WText(Wt::WString::fromUTF8("0"), ui->total_month_credit_table->elementAt(1, 5));
	    new Wt::WText(Wt::WString::fromUTF8(""), ui->total_month_credit_table->elementAt(1, 6));

		ui->total_month_credit_table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped");



	} else if (operation_name == "create") // ****************************************************************
	{
		ui->create_user_tab_mi->setHidden(false);
		//ui->main_tabs->setTabEnabled(0, 1);
		ui->main_tabs->setCurrentIndex(1);
	} else // operation_name == "edit" ***********************************************************************
	{
		//ui->edit_user_tab_mi->setHidden(false);
		//ui->main_tabs->setTabEnabled(0, 1);
		//ui->main_tabs->setCurrentIndex(2);


		//ui->user_treeTable->tree()


	    Wt::WMessageBox *messageBox;
		std::string changedSubscriberName = "";
		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = ui->user_treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			ui->edit_user_tab_mi->setHidden(false);
			ui->main_tabs->setCurrentIndex(2);

			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}


			changedSubscriberName = selected_node->label()->text().toUTF8();

			highlightedRows.clear();






			mysql_init(&mysql);
			conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
			if(conn==NULL)
			{
				std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
			}
			mysql_query(&mysql,"SET NAMES 'UTF8'");
			if(conn==NULL)
			{
				std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
			}




// get data from mysql tables and load to form elements

			std::string subscriber_name = "Габриель Джек Питерсон";


			// get all data from mysql tables
			std::string mysql_subscriber_all_table_data = "SELECT group_name, subscriber.subscriber_id, "
					"subscriber.group_id, name, surname, patronymic, full_name, subscriber_type, "
					"account_number, user_login, user_password, date_since, date_by, tariff_plan, "
					"user_email, city, district, street, housing, floor, house, apartment, "
					"index_number, contact_numbers, personal_code,organization_code, tax_code, "
					"account, bank_code, agreement, agreement_date, description, information "
					"FROM subscriber_group, subscriber, contacts, requisites, information_fields "
					"WHERE subscriber.full_name = '"+changedSubscriberName+"' AND subscriber_group.group_id = subscriber.group_id "
					"AND subscriber.subscriber_id = contacts.subscriber_id AND contacts.subscriber_id = requisites.subscriber_id "
					"AND requisites.subscriber_id = information_fields.subscriber_id";

			query_state=mysql_query(conn, mysql_subscriber_all_table_data.c_str());






			// set variables and form elements with data from mysql tables

			if(query_state!=0)
			{
			   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			}
			res=mysql_store_result(conn);
			std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
			row=mysql_fetch_row(res);


			//std::cout << row[0] << " *****************************************************" << std::endl;
			//std::cout << row[1] << " *****************************************************" << std::endl;
			//std::cout << row[2] << " *****************************************************" << std::endl;

// get all data from mysql tables

			// subscriber group info variable
			group_name_edit_mode = row[0];
			new_group_name_edit_mode = group_name_edit_mode;

			// subscriber contacts data variables
			subscriber_id_edit_mode = row[1];
			group_id_edit_mode = row[2];
			name_edit_mode= row[3];
			surname_edit_mode = row[4];
			patronymic_edit_mode = row[5];
			full_name_edit_mode = row[6];
			subscriber_type_edit_mode = row[7];
			account_number_edit_mode = row[8];
			user_login_edit_mode = row[9];
			user_password_edit_mode = row[10];
			date_since_edit_mode = row[11];
			date_by_edit_mode = row[12];
			tariff_plan_edit_mode  = row[13];
			user_email_edit_mode = row[14];

			// subscriber contacts data variables
			city_edit_mode = row[15];
			district_edit_mode = row[16];
			street_edit_mode = row[17];
			housing_edit_mode = row[18];
			house_floor_edit_mode = row[19];
			house_edit_mode = row[20];
			apartment_edit_mode = row[21];
			index_number_edit_mode = row[22];
			contact_numbers_edit_mode = row[23];

			// subscriber requisites data variables
			personal_code_edit_mode = row[24];
			organization_code_edit_mode = row[25];
			tax_code_edit_mode = row[26];
			account_edit_mode = row[27];
			bank_code_edit_mode = row[28];
			agreement_edit_mode = row[29];
			agreement_date_edit_mode = row[30];

			// subscriber information fields data variables
			description_edit_mode = row[31];
			information_edit_mode = row[32];

			// subscriber and contacts form control data load
			ui->user_type_combo_box_edit_user_tab->setValueText(Wt::WString::fromUTF8(subscriber_type_edit_mode));
			ui->account_number_edit_edit_user_tab->setText(Wt::WString::fromUTF8(account_number_edit_mode));
			ui->user_full_name_edit_edit_user_tab->setText(Wt::WString::fromUTF8(full_name_edit_mode));
			ui->user_group_edit_edit_user_tab->setText(Wt::WString::fromUTF8(group_name_edit_mode));
			ui->user_login_edit_edit_user_tab->setText(Wt::WString::fromUTF8(user_login_edit_mode));
			ui->user_pass_edit_edit_user_tab->setText(Wt::WString::fromUTF8(user_password_edit_mode));

			if(date_since_edit_mode == "" || date_since_edit_mode == "0000-00-00")
			{
				ui->user_include_date_checkbox_edit_user_tab->setChecked(false);
				ui->user_include_date_date_edit_edit_user_tab->setText("");
			}
			else
			{
				ui->user_include_date_checkbox_edit_user_tab->setChecked(true);
				ui->user_include_date_date_edit_edit_user_tab->setFormat("yyyy-MM-dd");
				ui->user_include_date_date_edit_edit_user_tab->setText(Wt::WString::fromUTF8(date_since_edit_mode));
			}

			if(date_by_edit_mode == "" || date_by_edit_mode == "0000-00-00")
			{
				ui->user_disable_date_checkbox_edit_user_tab->setChecked(false);
				ui->user_disable_date_date_edit_edit_user_tab->setText("");
			}
			else
			{
				ui->user_disable_date_checkbox_edit_user_tab->setChecked(true);
				ui->user_disable_date_date_edit_edit_user_tab->setFormat("yyyy-MM-dd");
				ui->user_disable_date_date_edit_edit_user_tab->setText(Wt::WString::fromUTF8(date_by_edit_mode));
			}

			ui->tariff_plan_edit_edit_user_tab->setText(Wt::WString::fromUTF8(tariff_plan_edit_mode));
			ui->user_address_text_area_edit_user_tab->setText(Wt::WString::fromUTF8("Город: " + city_edit_mode + "\n" + "Район: " + district_edit_mode+ "\n" +
					"Улица: " + street_edit_mode+ "\n" + "Корпус: " + housing_edit_mode+ "\n" + "Этаж:" + house_floor_edit_mode+ "\n" +
					"Дом:" + house_edit_mode+ "\n" + "Квартира: " + apartment_edit_mode+ "\n" + "Индекс: " + index_number_edit_mode));
			ui->user_email_edit_edit_user_tab->setText(Wt::WString::fromUTF8(user_email_edit_mode));
			ui->contact_number_edit_edit_user_tab->setText(Wt::WString::fromUTF8(contact_numbers_edit_mode));




			// ip_adreess entry load data
			std::string mysql_get_ip_address = "SELECT ip_address FROM ip_addresses WHERE ip_addresses.subscriber_id = '"+subscriber_id_edit_mode+"'";
			query_state=mysql_query(conn, mysql_get_ip_address.c_str());

			std::string new_ip_addresses = "";
			if(query_state!=0)
			{
			   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			}
			res=mysql_store_result(conn);
			row=mysql_fetch_row(res);
			while(row!=NULL)
			{
				new_ip_addresses.append(row[0]);
				row=mysql_fetch_row(res);
				if (row!=NULL)
				{
					new_ip_addresses.append(", ");
				}
			}

			ui->ip_login_edit_edit_user_tab->setText(Wt::WString::fromUTF8(new_ip_addresses));






			// phone number entry load data
			std::string mysql_get_phone_numbers = "SELECT number FROM phone_numbers WHERE phone_numbers.subscriber_id = '"+subscriber_id_edit_mode+"'";
			query_state=mysql_query(conn, mysql_get_phone_numbers.c_str());

			std::string new_phone_numbers= "";
			if(query_state!=0)
			{
			   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			}
			res=mysql_store_result(conn);
			row=mysql_fetch_row(res);
			while(row!=NULL)
			{
				new_phone_numbers.append(row[0]);
				row=mysql_fetch_row(res);
				if (row!=NULL)
				{
					new_phone_numbers.append(", ");
				}
			}

			ui->user_number_edit_edit_user_tab->setText(Wt::WString::fromUTF8(new_phone_numbers));





			// requisite form control load data
			ui->personal_code_edit_edit_user_tab->setValueText(Wt::WString::fromUTF8(personal_code_edit_mode));
			ui->organization_code_edit_edit_user_tab->setText(Wt::WString::fromUTF8(organization_code_edit_mode));
			ui->tax_code_edit_edit_user_tab->setText(Wt::WString::fromUTF8(tax_code_edit_mode));
			ui->requisite_account_edit_edit_user_tab->setText(Wt::WString::fromUTF8(account_edit_mode));
			ui->bank_code_edit_edit_user_tab->setText(Wt::WString::fromUTF8(bank_code_edit_mode));
			ui->agreement_edit_edit_user_tab->setText(Wt::WString::fromUTF8(agreement_edit_mode));

			if(agreement_date_edit_mode == "" || agreement_date_edit_mode == "0000-00-00")
			{
				ui->agreement_checkbox_edit_user_tab->setChecked(false);
				ui->agreement_date_edit_edit_user_tab->setText("");
			}
			else
			{
				ui->agreement_checkbox_edit_user_tab->setChecked(true);
				ui->agreement_date_edit_edit_user_tab->setFormat("yyyy-MM-dd");
				ui->agreement_date_edit_edit_user_tab->setText(Wt::WString::fromUTF8(agreement_date_edit_mode));
			}

			// information fields form control load data
			ui->description_text_area_edit_user_tab->setText(Wt::WString::fromUTF8(description_edit_mode));
			ui->information_text_area_edit_user_tab->setText(Wt::WString::fromUTF8(information_edit_mode));


/*
// clear all data variables

			// subscriber group info variable
			group_name_edit_mode = "";

			// subscriber contacts data variables
			subscriber_id_edit_mode = "";
			group_id_edit_mode = "";
			name_edit_mode = "";
			surname_edit_mode = "";
			patronymic_edit_mode = "";
			full_name_edit_mode = "";
			subscriber_type_edit_mode = "";
			account_number_edit_mode = "";
			user_login_edit_mode = "";
			user_password_edit_mode = "";
			date_since_edit_mode = "";
			date_by_edit_mode = "";
			tariff_plan_edit_mode  = "";
			user_email_edit_mode = "";

			// subscriber contacts data variables
			city_edit_mode = "";
			district_edit_mode = "";
			street_edit_mode = "";
			housing_edit_mode = "";
			house_floor_edit_mode = "";
			house_edit_mode = "";
			apartment_edit_mode = "";
			index_number_edit_mode = "";
			contact_numbers_edit_mode = "";

			// subscriber requisites data variables
			personal_code_edit_mode = "";
			organization_code_edit_mode = "";
			tax_code_edit_mode = "";
			account_edit_mode = "";
			bank_code_edit_mode = "";
			agreement_edit_mode = "";
			agreement_date_edit_mode = "";

			// subscriber information fields data variables
			description_edit_mode = "";
			information_edit_mode = "";
*/
			mysql_free_result(res);
			mysql_close(conn);

		} else
		{
			messageBox = new Wt::WMessageBox(Wt::WString::fromUTF8("Ошибка"), Wt::WString::fromUTF8("Не выбран абонент"), Wt::Information, Wt::Yes | Wt::No);
			messageBox->buttonClicked().connect(std::bind([=] () {
			delete messageBox;
			}));

			messageBox->show();
		}
	}


}





// dialog window for enter subscriber full name
extern void WtAccounts::subscriber_name_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Абонент"));

    dialog->resize(500, 350);

    Wt::WLabel *surname_label = new Wt::WLabel(Wt::WString::fromUTF8("Фамилия"),
				       dialog->contents());
    surname_label->setStyleClass("col-lg-4 col-md-4 top_pad");

    Wt::WContainerWidget *surname_edit_container = new Wt::WContainerWidget(dialog->contents());
    surname_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8"));
    surname_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *surname_edit = new Wt::WLineEdit(surname_edit_container);
    surname_label->setBuddy(surname_edit);



    Wt::WLabel *name_label = new Wt::WLabel(Wt::WString::fromUTF8("Имя"),
				       dialog->contents());
    name_label->setStyleClass("col-lg-4 col-md-4 top_mar top_pad");

    Wt::WContainerWidget *name_edit_container = new Wt::WContainerWidget(dialog->contents());
    name_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    name_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *name_edit = new Wt::WLineEdit(name_edit_container);
    name_label->setBuddy(name_edit);



    Wt::WLabel *family_name_label = new Wt::WLabel(Wt::WString::fromUTF8("Отчество"),
				       dialog->contents());
    family_name_label->setStyleClass("col-lg-4 col-md-4 top_mar top_pad");

    Wt::WContainerWidget *family_name_edit_container = new Wt::WContainerWidget(dialog->contents());
    family_name_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    family_name_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *family_name_edit = new Wt::WLineEdit(family_name_edit_container);
    family_name_label->setBuddy(family_name_edit);



    Wt::WLabel *full_name_label = new Wt::WLabel(Wt::WString::fromUTF8("Полное имя"),
				       dialog->contents());
    full_name_label->setStyleClass("col-lg-4 col-md-4 top_mar top_pad");

    Wt::WContainerWidget *full_name_edit_container = new Wt::WContainerWidget(dialog->contents());
    full_name_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    full_name_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *full_name_edit = new Wt::WLineEdit(full_name_edit_container);
    full_name_label->setBuddy(full_name_edit);

    // if edit mode then we fill dialog window entries with subscriber name info
    if(operation_name == "edit")
    {
    	surname_edit->setText(Wt::WString::fromUTF8(surname_edit_mode));
        name_edit->setText(Wt::WString::fromUTF8(name_edit_mode));
        family_name_edit->setText(Wt::WString::fromUTF8(patronymic_edit_mode));
        full_name_edit->setText(Wt::WString::fromUTF8(full_name_edit_mode));

    }


    dialog->contents()->addStyleClass("form-group");



    /*
    Wt::WRegExpValidator *validator =
        new Wt::WRegExpValidator("[A-Za-z][1-9][0-9]{0,2}");
    validator->setMandatory(true);
    edit->setValidator(validator);
     */
    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);
    //if (wApp->environment().ajax())
     // ok->disable();

    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();

    /*edit->keyWentUp().connect(std::bind([=] () {
	ok->setDisabled(edit->validate() != Wt::WValidator::Valid);
    }));*/

    /*
     * Accept the dialog
     */
    ok->clicked().connect(std::bind([=] () {
	//if (edit->validate())
	    dialog->accept();
    }));

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
		if (operation_name == "create")
		{
			name = name_edit->text().toUTF8();
			surname = surname_edit->text().toUTF8();
			patronymic = family_name_edit->text().toUTF8();
			ui->user_full_name_edit->setText(surname_edit->text() + " " + name_edit->text() + " " + family_name_edit->text());
		} else // operation_name == "edit"
		{
			if (name_edit->text() != "") name_edit_mode = name_edit->text().toUTF8();
			if (surname_edit->text() != "") surname_edit_mode = surname_edit->text().toUTF8();
			if (family_name_edit->text() != "") patronymic_edit_mode = family_name_edit->text().toUTF8();
			ui->user_full_name_edit_edit_user_tab->setText(Wt::WString::fromUTF8(surname_edit_mode) + " " + Wt::WString::fromUTF8(name_edit_mode) + " " + Wt::WString::fromUTF8(patronymic_edit_mode));
		}
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where we can chose subscriber group
extern void WtAccounts::subscriber_group_dialog(std::string operation_name)
{
	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");

    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Выберите группу абонента:"));

    dialog->resize(500, 650);

// dialog contents
    Wt::WTreeTable *treeTable;
	treeTable = new Wt::WTreeTable(dialog->contents());
    //treeTable->resize(650, 300);
    treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
    treeTable->tree()->setSelectionMode(Wt::SingleSelection);
    treeTable->addColumn(Wt::WString::fromUTF8("Абонентов"), 100);

    Wt::WTreeTableNode *tree_root;
	tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("ТОО"));
    treeTable->setTreeRoot(tree_root, Wt::WString::fromUTF8("Группа"));

    Wt::WTreeTableNode *tree_node;

// add node form mysql table
	query_state=mysql_query(conn, "select * from subscriber_group");
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tree_node  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[1]), 0, tree_root);
		tree_node ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(row[2])));
	}

	mysql_free_result(res);
	mysql_close(conn);

	tree_root->expand();



// dialog footer
    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();


    /*
     * Accept the dialog
     */
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}
			if (operation_name == "create")
			{
				ui->user_group_edit->setText(selected_node->label()->text());
				group_name = selected_node->label()->text().toUTF8();
			} else // operation_name == "edit"
			{
				ui->user_group_edit_edit_user_tab->setText(selected_node->label()->text());
				new_group_name_edit_mode = selected_node->label()->text().toUTF8();
			}
			highlightedRows.clear();
		}
	}

	delete dialog;
    }));


    dialog->show();
}





// show dialog window where you create new phone numbers
extern void WtAccounts::create_phone_number_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Новый телефонный номер"));

    dialog->resize(600, 550);



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	/*
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	 */
	mysql_query(&mysql,"SET NAMES 'UTF8'");



    // device label and edit
    Wt::WLabel *device_label = new Wt::WLabel(Wt::WString::fromUTF8("Устройство"),
    				       dialog->contents());
    device_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *device_edit_container = new Wt::WContainerWidget(dialog->contents());
    device_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    device_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *device_edit = new Wt::WLineEdit(device_edit_container);
    device_edit->setText(Wt::WString::fromUTF8("Учет телефонных вызовов"));
    device_label->setBuddy(device_edit);



    // range label and combo box
    Wt::WLabel *range_label = new Wt::WLabel(Wt::WString::fromUTF8("Диапазон"),
    				       dialog->contents());
    range_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *range_combobox_container = new Wt::WContainerWidget(dialog->contents());
    range_combobox_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    range_combobox_container->setHtmlTagName("div");

    Wt::WComboBox *range_combobox = new Wt::WComboBox(range_combobox_container);
    range_label->setBuddy(range_combobox);



    // number label and two edit
    Wt::WLabel *number_label = new Wt::WLabel(Wt::WString::fromUTF8("Номер"),
    				       dialog->contents());
    number_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WContainerWidget *number_prefix_edit_container = new Wt::WContainerWidget(dialog->contents());
    number_prefix_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-2 col-lg-2 top_mar"));
    number_prefix_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *number_prefix_edit = new Wt::WLineEdit(number_prefix_edit_container);

    Wt::WContainerWidget *number_edit_container = new Wt::WContainerWidget(dialog->contents());
    number_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-7 col-lg-7 top_mar"));
    number_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *number_edit = new Wt::WLineEdit(number_edit_container);
    number_label->setBuddy(number_edit);



    // description label and text area
    Wt::WLabel *description_label = new Wt::WLabel(Wt::WString::fromUTF8("Описание"),
    				       dialog->contents());
    description_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *description_text_area_container = new Wt::WContainerWidget(dialog->contents());
    description_text_area_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    description_text_area_container->setHtmlTagName("div");

    Wt::WTextArea *description_text_area = new Wt::WTextArea(description_text_area_container);
    description_text_area->setColumns(80);
    description_text_area->setRows(3);
    description_label->setBuddy(description_text_area);



    // include date label, check box and date edit
    Wt::WLabel *include_date_label = new Wt::WLabel(Wt::WString::fromUTF8("Включен с"),
    				       dialog->contents());
    include_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *include_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    include_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *include_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    include_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    include_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *include_date_edit = new Wt::WDateEdit(include_date_edit_container);
    include_date_edit->setFormat("yyyy-MM-dd");
    include_date_label->setBuddy(include_date_edit);



    // disable date label, check box and date edit
    Wt::WLabel *disable_date_label = new Wt::WLabel(Wt::WString::fromUTF8("По"),
    				       dialog->contents());
    disable_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *disable_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    disable_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *disable_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    disable_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    disable_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *disable_date_edit = new Wt::WDateEdit(disable_date_edit_container);
    disable_date_edit->setFormat("yyyy-MM-dd");
    disable_date_label->setBuddy(disable_date_edit);



    // tariff plan label and combo box
    Wt::WLabel *tariff_plan_label = new Wt::WLabel(Wt::WString::fromUTF8("Тарифный план"),
    				       dialog->contents());
    tariff_plan_label->setStyleClass("col-lg-3 col-md-3 top_pad");

    Wt::WContainerWidget *tariff_plan_combobox_container = new Wt::WContainerWidget(dialog->contents());
    tariff_plan_combobox_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    tariff_plan_combobox_container->setHtmlTagName("div");

    Wt::WComboBox *tariff_plan_combobox = new Wt::WComboBox(tariff_plan_combobox_container);
    tariff_plan_label->setBuddy(tariff_plan_combobox);

    query_state=mysql_query(conn, "SELECT plan_name FROM tariff_plan");

	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tariff_plan_combobox->addItem(Wt::WString::fromUTF8(row[0]));
	}

	mysql_free_result(res);
	mysql_close(conn);

    dialog->contents()->addStyleClass("form-group");



    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{


		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		/*
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		 */
		mysql_query(&mysql,"SET NAMES 'UTF8'");


		std::string device = device_edit->text().toUTF8();
		std::string number = (number_prefix_edit->text() + number_edit->text()).toUTF8();
		std::string description = description_text_area->text().toUTF8();

		std::string date_since = "";
		if (include_date_checkbox->isChecked())
		{
			date_since = boost::lexical_cast<std::string>(include_date_edit->text());
		}

		std::string date_by = "";
		if (disable_date_checkbox->isChecked())
		{
			date_by = boost::lexical_cast<std::string>(disable_date_edit->text());
		}

		std::string tariff_plan = tariff_plan_combobox->currentText().toUTF8();

		std::string mysql_insert_new_number = "";

		if (operation_name == "create")
		{
			mysql_insert_new_number = "insert into phone_numbers (subscriber_id, device, number, "
					"description, date_since, date_by, tariff_plan, number_status)  "
					"values (NULL, '"+device+"', '"+number+"', '"+description+"', '"+date_since+"', "
							"'"+date_by+"', '"+tariff_plan+"', 'not_processed')";
		} else // operation_name == "edit"
		{
			mysql_insert_new_number = "insert into phone_numbers (subscriber_id, device, number, "
					"description, date_since, date_by, tariff_plan, number_status)  "
					"values ('"+subscriber_id_edit_mode+"', '"+device+"', '"+number+"', '"+description+"', '"+date_since+"', "
							"'"+date_by+"', '"+tariff_plan+"', 'processed')";
		}

		query_state=mysql_query(conn, mysql_insert_new_number.c_str());
		//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
		//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}


		phone_number_table->clear();
		phone_number_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер")));
		phone_number_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
		phone_number_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
		phone_number_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
		phone_number_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тарифный план")));
		phone_number_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

		int row_number = 0;


		std::string mysql_phone_number_table_data = "";

		if (operation_name == "create")
		{
			mysql_phone_number_table_data = "select device, number, description, date_since, date_by, "
					  "tariff_plan from phone_numbers where phone_numbers.number_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_phone_number_table_data = "select device, number, description, date_since, date_by, "
					  "tariff_plan from phone_numbers where phone_numbers.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}


		query_state=mysql_query(conn, mysql_phone_number_table_data.c_str());
		//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
		//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[1]), phone_number_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[0]), phone_number_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), phone_number_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), phone_number_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), phone_number_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), phone_number_table->elementAt(row_number, 5));
		}
		phone_number_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where you delete exist phone numbers
extern void WtAccounts::delete_phone_number_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Удалить телефонный номер"));

    dialog->resize(600, 550);



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	/*
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	 */
	mysql_query(&mysql,"SET NAMES 'UTF8'");



    Wt::WTreeTable *treeTable;
	treeTable = new Wt::WTreeTable(dialog->contents());
    //treeTable->resize(650, 300);
    treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
    treeTable->tree()->setSelectionMode(Wt::SingleSelection);
    //treeTable->addColumn(Wt::WString::fromUTF8("Телефонные номера"), 100);

    Wt::WTreeTableNode *tree_root;
	tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("Номера"));
    treeTable->setTreeRoot(tree_root, Wt::WString::fromUTF8("Телефонные номера"));

    Wt::WTreeTableNode *tree_node;

    std::string mysql_get_numbers = "";

// add node form mysql table
	if (operation_name == "create")
	{
		mysql_get_numbers = "SELECT number FROM phone_numbers WHERE number_status = 'not_processed'";
	} else // operation_name == "edit"
	{
		mysql_get_numbers = "SELECT number FROM phone_numbers WHERE  subscriber_id = '"+subscriber_id_edit_mode+"'";
	}
	query_state=mysql_query(conn, mysql_get_numbers.c_str());
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tree_node  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[0]), 0, tree_root);
		//tree_node ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(row[2])));
	}

	mysql_free_result(res);
	mysql_close(conn);

	tree_root->expand();



    Wt::WPushButton *ok = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{


		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		/*
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		 */
		mysql_query(&mysql,"SET NAMES 'UTF8'");



		std::string mysql_delete_number = "";
		std::string selected_number = "";

		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}

			selected_number = selected_node->label()->text().toUTF8();

			if (operation_name == "create")
			{
				mysql_delete_number = "DELETE FROM phone_numbers WHERE number = '"+selected_number+"'";
			} else // operation_name == "edit"
			{
				mysql_delete_number = "DELETE FROM phone_numbers WHERE number = '"+selected_number+"' AND subscriber_id = '"+subscriber_id_edit_mode+"'";
			}
			highlightedRows.clear();
		}


		query_state=mysql_query(conn, mysql_delete_number.c_str());

		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}



		phone_number_table->clear();
		phone_number_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер")));
		phone_number_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
		phone_number_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
		phone_number_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
		phone_number_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тарифный план")));
		phone_number_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

		int row_number = 0;


		std::string mysql_phone_number_table_data = "";

		if (operation_name == "create")
		{
			mysql_phone_number_table_data = "select device, number, description, date_since, date_by, "
					  "tariff_plan from phone_numbers where phone_numbers.number_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_phone_number_table_data = "select device, number, description, date_since, date_by, "
					  "tariff_plan from phone_numbers where phone_numbers.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}


		query_state=mysql_query(conn, mysql_phone_number_table_data.c_str());
		//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
		//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[1]), phone_number_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[0]), phone_number_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), phone_number_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), phone_number_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), phone_number_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), phone_number_table->elementAt(row_number, 5));
		}
		phone_number_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where you manage (create, delete, change etc) phone numbers
extern void WtAccounts::subscriber_phone_number_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Телефонные номера"));

    dialog->resize(1050, 450);



    // top buttons
    Wt::WPushButton *save_exit_button = new Wt::WPushButton(Wt::WString::fromUTF8("Сохранить и закрыть"), dialog->contents());
    save_exit_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    Wt::WPushButton *add_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Добавить"), dialog->contents());
    add_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *change_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Изменить"), dialog->contents());
    change_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *delete_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->contents());
    delete_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *set_asmain_button = new Wt::WPushButton(Wt::WString::fromUTF8("Установить как основной"), dialog->contents());
    set_asmain_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    Wt::WPushButton *only_active_button = new Wt::WPushButton(Wt::WString::fromUTF8("Только активные"), dialog->contents());
    only_active_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));



	add_ip_button->clicked().connect(boost::bind(&WtAccounts::create_phone_number_dialog, this, operation_name));
	delete_ip_button->clicked().connect(boost::bind(&WtAccounts::delete_phone_number_dialog, this, operation_name));



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	/*
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	 */
	mysql_query(&mysql,"SET NAMES 'UTF8'");



	// table with data about ip address
	phone_number_table = new Wt::WTable(dialog->contents());
	phone_number_table->setHeaderCount(1);
	phone_number_table->setWidth(Wt::WLength("100%"));

	phone_number_table->setHeaderCount(1);

	phone_number_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер")));
	phone_number_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
	phone_number_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
	phone_number_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
	phone_number_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тарифный план")));
	phone_number_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

	int row_number = 0;

	std::string mysql_phone_number_table_data = "";

	if (operation_name == "create")
	{
		mysql_phone_number_table_data = "select device, number, description, date_since, date_by, "
				  "tariff_plan from phone_numbers where phone_numbers.number_status = 'not_processed'";
	} else // operation_name == "edit"
	{
		mysql_phone_number_table_data = "select device, number, description, date_since, date_by, "
				  "tariff_plan from phone_numbers where phone_numbers.subscriber_id = '"+subscriber_id_edit_mode+"'";
	}

	query_state=mysql_query(conn, mysql_phone_number_table_data.c_str());
	//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
	//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		row_number++;
		new Wt::WText(Wt::WString::fromUTF8(row[1]), phone_number_table->elementAt(row_number, 0));
		new Wt::WText(Wt::WString::fromUTF8(row[0]), phone_number_table->elementAt(row_number, 1));
		new Wt::WText(Wt::WString::fromUTF8(row[3]), phone_number_table->elementAt(row_number, 2));
		new Wt::WText(Wt::WString::fromUTF8(row[4]), phone_number_table->elementAt(row_number, 3));
		new Wt::WText(Wt::WString::fromUTF8(row[5]), phone_number_table->elementAt(row_number, 4));
		new Wt::WText(Wt::WString::fromUTF8(row[2]), phone_number_table->elementAt(row_number, 5));
	}
	row_number = 0;
	phone_number_table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped top_mar top_pad");


	mysql_free_result(res);
	mysql_close(conn);;



    dialog->contents()->addStyleClass("form-group");


    //Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    //ok->setDefault(true);

    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();


    // Accept the dialog
    save_exit_button->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		/*
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		 */
		mysql_query(&mysql,"SET NAMES 'UTF8'");


		// click button save and exit get new ip addresses and write in ip address entry in main form
		std::string mysql_get_number = "";

		if (operation_name == "create")
		{
			mysql_get_number = "SELECT number FROM phone_numbers WHERE phone_numbers.number_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_get_number = "SELECT number FROM phone_numbers WHERE phone_numbers.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}


		query_state=mysql_query(conn, mysql_get_number.c_str());

		std::string new_phone_numbers = "";
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		row=mysql_fetch_row(res);
		while(row!=NULL)
		{
			new_phone_numbers.append(row[0]);
			row=mysql_fetch_row(res);
			if (row!=NULL)
			{
				new_phone_numbers.append(", ");
			}
		}
		mysql_free_result(res);
		mysql_close(conn);

		if (operation_name == "create")
		{
			ui->user_number_edit->setText(Wt::WString::fromUTF8(new_phone_numbers));
		} else // operation_name == "edit"
		{
			ui->user_number_edit_edit_user_tab->setText(Wt::WString::fromUTF8(new_phone_numbers));
		}
	}


	delete dialog;
    }));

    dialog->show();
}






// show dialog window where you create new ip addresses
extern void WtAccounts::create_ip_address_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Новый IP адрес"));

    dialog->resize(600, 550);

	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");



    // device label and edit
    Wt::WLabel *device_label = new Wt::WLabel(Wt::WString::fromUTF8("Устройство"),
    				       dialog->contents());
    device_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *device_edit_container = new Wt::WContainerWidget(dialog->contents());
    device_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    device_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *device_edit = new Wt::WLineEdit(device_edit_container);
    device_edit->setText(Wt::WString::fromUTF8("Netflow"));
    device_label->setBuddy(device_edit);



    // range label and combo box
    Wt::WLabel *range_label = new Wt::WLabel(Wt::WString::fromUTF8("Диапазон"),
    				       dialog->contents());
    range_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *range_combobox_container = new Wt::WContainerWidget(dialog->contents());
    range_combobox_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    range_combobox_container->setHtmlTagName("div");

    Wt::WComboBox *range_combobox = new Wt::WComboBox(range_combobox_container);
    range_label->setBuddy(range_combobox);



    // ip label and edit
    Wt::WLabel *ip_label = new Wt::WLabel(Wt::WString::fromUTF8("IP"),
    				       dialog->contents());
    ip_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *ip_edit_container = new Wt::WContainerWidget(dialog->contents());
    ip_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    ip_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *ip_edit = new Wt::WLineEdit(ip_edit_container);
    ip_label->setBuddy(ip_edit);



    // description label and text area
    Wt::WLabel *description_label = new Wt::WLabel(Wt::WString::fromUTF8("Описание"),
    				       dialog->contents());
    description_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *description_text_area_container = new Wt::WContainerWidget(dialog->contents());
    description_text_area_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    description_text_area_container->setHtmlTagName("div");

    Wt::WTextArea *description_text_area = new Wt::WTextArea(description_text_area_container);
    description_text_area->setColumns(80);
    description_text_area->setRows(3);
    description_label->setBuddy(description_text_area);



    // include date label, check box and date edit
    Wt::WLabel *include_date_label = new Wt::WLabel(Wt::WString::fromUTF8("Включен с"),
    				       dialog->contents());
    include_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *include_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    include_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *include_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    include_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    include_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *include_date_edit = new Wt::WDateEdit(include_date_edit_container);
    include_date_edit->setFormat("yyyy-MM-dd");
    include_date_label->setBuddy(include_date_edit);



    // disable date label, check box and date edit
    Wt::WLabel *disable_date_label = new Wt::WLabel(Wt::WString::fromUTF8("По"),
    				       dialog->contents());
    disable_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *disable_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    disable_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *disable_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    disable_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    disable_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *disable_date_edit = new Wt::WDateEdit(disable_date_edit_container);
    disable_date_edit->setFormat("yyyy-MM-dd");
    disable_date_label->setBuddy(disable_date_edit);



    // tariff plan label and combo box
    Wt::WLabel *tariff_plan_label = new Wt::WLabel(Wt::WString::fromUTF8("Тарифный план"),
    				       dialog->contents());
    tariff_plan_label->setStyleClass("col-lg-3 col-md-3 top_pad");

    Wt::WContainerWidget *tariff_plan_combobox_container = new Wt::WContainerWidget(dialog->contents());
    tariff_plan_combobox_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    tariff_plan_combobox_container->setHtmlTagName("div");

    Wt::WComboBox *tariff_plan_combobox = new Wt::WComboBox(tariff_plan_combobox_container);
    tariff_plan_label->setBuddy(tariff_plan_combobox);

    query_state=mysql_query(conn, "SELECT plan_name FROM tariff_plan");

	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tariff_plan_combobox->addItem(Wt::WString::fromUTF8(row[0]));
	}

	mysql_free_result(res);
	mysql_close(conn);





    dialog->contents()->addStyleClass("form-group");


    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{


		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");


		std::string device = device_edit->text().toUTF8();
		std::string ip_address = ip_edit->text().toUTF8();
		std::string description = description_text_area->text().toUTF8();

		std::string date_since = "";
		if (include_date_checkbox->isChecked())
		{
			date_since = boost::lexical_cast<std::string>(include_date_edit->text());
		}

		std::string date_by = "";
		if (disable_date_checkbox->isChecked())
		{
			date_by = boost::lexical_cast<std::string>(disable_date_edit->text());
		}

		std::string tariff_plan = tariff_plan_combobox->currentText().toUTF8();

		std::string mysql_insert_new_ip = "";



		if (operation_name == "create")
		{
			mysql_insert_new_ip = "insert into ip_addresses (subscriber_id, device, ip_address, "
							"description, date_since, date_by, tariff_plan, ip_status)  "
							"values (NULL, '"+device+"', '"+ip_address+"', "
									"'"+description+"', '"+date_since+"', '"+date_by+"', '"+tariff_plan+"', 'not_processed')";
		} else // operation_name == "edit"
		{
			mysql_insert_new_ip = "insert into ip_addresses (subscriber_id, device, ip_address, "
							"description, date_since, date_by, tariff_plan, ip_status)  "
							"values ('"+subscriber_id_edit_mode+"', '"+device+"', '"+ip_address+"', "
									"'"+description+"', '"+date_since+"', '"+date_by+"', '"+tariff_plan+"', 'processed')";
		}

		query_state=mysql_query(conn, mysql_insert_new_ip.c_str());
		//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
		//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}


		ip_address_table->clear();
		ip_address_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("IP адрес")));
		ip_address_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
		ip_address_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
		ip_address_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
		ip_address_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тарифный план")));
		ip_address_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

		int row_number = 0;


		std::string mysql_ip_address_table_data = "";

		if (operation_name == "create")
		{
			mysql_ip_address_table_data = "select device, ip_address, description, date_since, date_by, "
					  "tariff_plan from ip_addresses where ip_addresses.ip_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_ip_address_table_data = "select device, ip_address, description, date_since, date_by, "
					  "tariff_plan from ip_addresses where ip_addresses.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}


		query_state=mysql_query(conn, mysql_ip_address_table_data.c_str());
		//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
		//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[1]), ip_address_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[0]), ip_address_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), ip_address_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), ip_address_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), ip_address_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), ip_address_table->elementAt(row_number, 5));
		}
		ip_address_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where you delete exist ip addresses
extern void WtAccounts::delete_ip_address_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Удалить IP адресс"));

    dialog->resize(600, 550);



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	/*
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	 */
	mysql_query(&mysql,"SET NAMES 'UTF8'");



    Wt::WTreeTable *treeTable;
	treeTable = new Wt::WTreeTable(dialog->contents());
    //treeTable->resize(650, 300);
    treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
    treeTable->tree()->setSelectionMode(Wt::SingleSelection);
    //treeTable->addColumn(Wt::WString::fromUTF8("Телефонные номера"), 100);

    Wt::WTreeTableNode *tree_root;
	tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("Адреса"));
    treeTable->setTreeRoot(tree_root, Wt::WString::fromUTF8("IP адреса"));

    Wt::WTreeTableNode *tree_node;

    std::string mysql_get_ip_addresses = "";

// add node form mysql table
	if (operation_name == "create")
	{
		mysql_get_ip_addresses = "SELECT ip_address FROM ip_addresses WHERE ip_status = 'not_processed'";
	} else // operation_name == "edit"
	{
		mysql_get_ip_addresses = "SELECT ip_address FROM ip_addresses WHERE  subscriber_id = '"+subscriber_id_edit_mode+"'";
	}
	query_state=mysql_query(conn, mysql_get_ip_addresses.c_str());
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tree_node  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[0]), 0, tree_root);
		//tree_node ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(row[2])));
	}

	mysql_free_result(res);
	mysql_close(conn);

	tree_root->expand();



    Wt::WPushButton *ok = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{


		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		/*
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		 */
		mysql_query(&mysql,"SET NAMES 'UTF8'");



		std::string mysql_delete_ip_address = "";
		std::string selected_ip_address = "";

		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}

			selected_ip_address= selected_node->label()->text().toUTF8();

			if (operation_name == "create")
			{
				mysql_delete_ip_address = "DELETE FROM ip_addresses WHERE ip_address = '"+selected_ip_address+"'";
			} else // operation_name == "edit"
			{
				mysql_delete_ip_address = "DELETE FROM ip_addresses WHERE ip_address = '"+selected_ip_address+"' AND subscriber_id = '"+subscriber_id_edit_mode+"'";
			}
			highlightedRows.clear();
		}


		query_state=mysql_query(conn, mysql_delete_ip_address.c_str());

		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}



		ip_address_table->clear();
		ip_address_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("IP адрес")));
		ip_address_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
		ip_address_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
		ip_address_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
		ip_address_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тарифный план")));
		ip_address_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

		int row_number = 0;


		std::string mysql_ip_addresses_table_data = "";

		if (operation_name == "create")
		{
			mysql_ip_addresses_table_data = "select device, ip_address, description, date_since, date_by, "
					  "tariff_plan from ip_addresses where ip_addresses.ip_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_ip_addresses_table_data = "select device, ip_address, description, date_since, date_by, "
					  "tariff_plan from ip_addresses where ip_addresses.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}


		query_state=mysql_query(conn, mysql_ip_addresses_table_data.c_str());
		//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
		//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[1]), ip_address_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[0]), ip_address_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), ip_address_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), ip_address_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), ip_address_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), ip_address_table->elementAt(row_number, 5));
		}
		ip_address_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where you manage (create, delete, change etc) subscriber ip addresses
extern void WtAccounts::subscriber_ip_address_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("IP адреса"));

    dialog->resize(1050, 450);



    // top buttons
    Wt::WPushButton *save_exit_button = new Wt::WPushButton(Wt::WString::fromUTF8("Сохранить и закрыть"), dialog->contents());
    save_exit_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    Wt::WPushButton *add_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Добавить"), dialog->contents());
    add_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *change_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Изменить"), dialog->contents());
    change_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *delete_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->contents());
    delete_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *set_asmain_button = new Wt::WPushButton(Wt::WString::fromUTF8("Установить как основной"), dialog->contents());
    set_asmain_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    Wt::WPushButton *only_active_button = new Wt::WPushButton(Wt::WString::fromUTF8("Только активные"), dialog->contents());
    only_active_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));


	add_ip_button->clicked().connect(boost::bind(&WtAccounts::create_ip_address_dialog, this, operation_name));
	delete_ip_button->clicked().connect(boost::bind(&WtAccounts::delete_ip_address_dialog, this, operation_name));






	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");




	// table with data about ip address
	ip_address_table = new Wt::WTable(dialog->contents());
	ip_address_table->setHeaderCount(1);
	ip_address_table->setWidth(Wt::WLength("100%"));

	ip_address_table->setHeaderCount(1);

	ip_address_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("IP адрес")));
	ip_address_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Устройство")));
	ip_address_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
	ip_address_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
	ip_address_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тарифный план")));
	ip_address_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

	int row_number = 0;



	std::string mysql_ip_address_table_data = "";

	if (operation_name == "create")
	{
		mysql_ip_address_table_data = "select device, ip_address, description, date_since, date_by, "
				  "tariff_plan from ip_addresses where ip_addresses.ip_status = 'not_processed'";
	} else // operation_name == "edit"
	{
		mysql_ip_address_table_data = "select device, ip_address, description, date_since, date_by, "
				  "tariff_plan from ip_addresses where ip_addresses.subscriber_id = '"+subscriber_id_edit_mode+"'";
	}

	query_state=mysql_query(conn, mysql_ip_address_table_data.c_str());
	//query_state=mysql_query(conn, "select device, ip_address, description, date_since, date_by, "
	//							  "tariff_plan from ip_addresses where ip_addresses.ip_status = '"not_processed"'");
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		row_number++;
		new Wt::WText(Wt::WString::fromUTF8(row[1]), ip_address_table->elementAt(row_number, 0));
		new Wt::WText(Wt::WString::fromUTF8(row[0]), ip_address_table->elementAt(row_number, 1));
		new Wt::WText(Wt::WString::fromUTF8(row[3]), ip_address_table->elementAt(row_number, 2));
		new Wt::WText(Wt::WString::fromUTF8(row[4]), ip_address_table->elementAt(row_number, 3));
		new Wt::WText(Wt::WString::fromUTF8(row[5]), ip_address_table->elementAt(row_number, 4));
		new Wt::WText(Wt::WString::fromUTF8(row[2]), ip_address_table->elementAt(row_number, 5));
	}
	row_number = 0;
	ip_address_table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped top_mar top_pad");


	mysql_free_result(res);
	mysql_close(conn);;




    dialog->contents()->addStyleClass("form-group");



    //Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    //ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    save_exit_button->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		/*
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		 */
		mysql_query(&mysql,"SET NAMES 'UTF8'");


		// click button save and exit get new ip addresses and write in ip address entry in main form
		std::string mysql_get_ip_address = "";

		if (operation_name == "create")
		{
			mysql_get_ip_address = "SELECT ip_address FROM ip_addresses WHERE ip_addresses.ip_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_get_ip_address = "SELECT ip_address FROM ip_addresses WHERE ip_addresses.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}


		query_state=mysql_query(conn, mysql_get_ip_address.c_str());

		std::string new_ip_addresses = "";
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		row=mysql_fetch_row(res);
		while(row!=NULL)
		{
			new_ip_addresses.append(row[0]);
			row=mysql_fetch_row(res);
			if (row!=NULL)
			{
				new_ip_addresses.append(", ");
			}
		}
		mysql_free_result(res);
		mysql_close(conn);
		if (operation_name == "create")
		{
			ui->ip_login_edit->setText(Wt::WString::fromUTF8(new_ip_addresses));
		} else // operation_name == "edit"
		{
			ui->ip_login_edit_edit_user_tab->setText(Wt::WString::fromUTF8(new_ip_addresses));
		}
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where you can chose tariff plan
extern void WtAccounts::subscriber_tariff_plan_dialog(std::string operation_name)
{
	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	 mysql_query(&mysql,"SET NAMES 'UTF8'");



    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Выберите тарифный план абонента:"));

    dialog->resize(500, 450);

// dialog contents
    Wt::WTreeTable *treeTable;
	treeTable = new Wt::WTreeTable(dialog->contents());
    //treeTable->resize(650, 300);
    treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
    treeTable->tree()->setSelectionMode(Wt::SingleSelection);
    //treeTable->addColumn(Wt::WString::fromUTF8("Абонентов"), 100);


    Wt::WTreeTableNode *tree_root;
	tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("Тарифные планы"));
    treeTable->setTreeRoot(tree_root, Wt::WString::fromUTF8("Тарифы"));

    Wt::WTreeTableNode *tree_node;



// add node form mysql table
	query_state=mysql_query(conn, "select * from tariff_plan");
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tree_node  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[1]), 0, tree_root);
		//tree_node ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(row[2])));
	}


	mysql_free_result(res);
	mysql_close(conn);

	tree_root->expand();



// dialog footer
    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();


    /*
     * Accept the dialog
     */
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}
			if (operation_name == "create")
			{
				ui->tariff_plan_edit->setText(selected_node->label()->text());
				tariff_plan = selected_node->label()->text().toUTF8();
			} else // operation_name == "edit"
			{
				ui->tariff_plan_edit_edit_user_tab->setText(selected_node->label()->text());
				tariff_plan_edit_mode = selected_node->label()->text().toUTF8();
			}
			highlightedRows.clear();
		}
	}

	delete dialog;
    }));


    dialog->show();
}





// show dialog window where fill subscriber address information or change
extern void WtAccounts::subscriber_contact_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Адресс"));

    dialog->resize(650, 400);



// city edit and label
    Wt::WLabel *city_label = new Wt::WLabel(Wt::WString::fromUTF8("Город"),
				       dialog->contents());
    city_label->setStyleClass("col-lg-2 col-md-2 top_pad");

    Wt::WContainerWidget *city_edit_container = new Wt::WContainerWidget(dialog->contents());
    city_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-4 col-lg-4"));
    city_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *city_edit = new Wt::WLineEdit(city_edit_container);
    city_label->setBuddy(city_edit);



// index_number edit and label
     Wt::WLabel *index_number_label = new Wt::WLabel(Wt::WString::fromUTF8("Индекс"),
    				       dialog->contents());
     index_number_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

     Wt::WContainerWidget *index_number_edit_container = new Wt::WContainerWidget(dialog->contents());
     index_number_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-4 col-lg-4 top_mar"));
     index_number_edit_container->setHtmlTagName("div");
     Wt::WLineEdit *index_number_edit = new Wt::WLineEdit(index_number_edit_container);
     index_number_label->setBuddy(index_number_edit);



// district edit and label
    Wt::WLabel *district_label = new Wt::WLabel(Wt::WString::fromUTF8("Район"),
				       dialog->contents());
    district_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

    Wt::WContainerWidget *district_edit_container = new Wt::WContainerWidget(dialog->contents());
    district_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-10 col-lg-10 top_mar"));
    district_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *district_edit = new Wt::WLineEdit(district_edit_container);
    district_label->setBuddy(district_edit);



// street edit and label
    Wt::WLabel *street_label = new Wt::WLabel(Wt::WString::fromUTF8("Улица"),
				       dialog->contents());
    street_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

    Wt::WContainerWidget *street_edit_container = new Wt::WContainerWidget(dialog->contents());
    street_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-10 col-lg-10 top_mar"));
    street_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *street_edit = new Wt::WLineEdit(street_edit_container);
    street_label->setBuddy(street_edit);



// housing edit and label
    Wt::WLabel *housing_label = new Wt::WLabel(Wt::WString::fromUTF8("Корпус"),
				       dialog->contents());
    housing_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

    Wt::WContainerWidget *housing_edit_container = new Wt::WContainerWidget(dialog->contents());
    housing_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-4 col-lg-4 top_mar"));
    housing_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *housing_edit = new Wt::WLineEdit(housing_edit_container);
    housing_label->setBuddy(housing_edit);



// house edit and label
     Wt::WLabel *house_label = new Wt::WLabel(Wt::WString::fromUTF8("Дом"),
    				       dialog->contents());
     house_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

     Wt::WContainerWidget *house_edit_container = new Wt::WContainerWidget(dialog->contents());
     house_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-4 col-lg-4 top_mar"));
     house_edit_container->setHtmlTagName("div");
     Wt::WLineEdit *house_edit = new Wt::WLineEdit(house_edit_container);
     house_label->setBuddy(house_edit);



// floor edit and label
    Wt::WLabel *floor_label = new Wt::WLabel(Wt::WString::fromUTF8("Этаж"),
				       dialog->contents());
    floor_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

    Wt::WContainerWidget *floor_edit_container = new Wt::WContainerWidget(dialog->contents());
    floor_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-4 col-lg-4 top_mar"));
    floor_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *floor_edit = new Wt::WLineEdit(floor_edit_container);
    floor_label->setBuddy(floor_edit);



// apartment edit and label
    Wt::WLabel *apartment_label = new Wt::WLabel(Wt::WString::fromUTF8("Квартира"),
				       dialog->contents());
    apartment_label->setStyleClass("col-lg-2 col-md-2 top_mar top_pad");

    Wt::WContainerWidget *apartment_edit_container = new Wt::WContainerWidget(dialog->contents());
    apartment_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-4 col-lg-4 top_mar"));
    apartment_edit_container->setHtmlTagName("div");
    Wt::WLineEdit *apartment_edit = new Wt::WLineEdit(apartment_edit_container);
    apartment_label->setBuddy(apartment_edit);


    // if edit mode thwn we fill dialog window's entries with contact data about subscriber
    if (operation_name == "edit")
    {
    	    city_edit->setText(Wt::WString::fromUTF8(city_edit_mode));
    	    index_number_edit->setText(Wt::WString::fromUTF8(index_number_edit_mode));
    	    district_edit->setText(Wt::WString::fromUTF8(district_edit_mode));
    	    street_edit->setText(Wt::WString::fromUTF8(street_edit_mode));
    	    housing_edit->setText(Wt::WString::fromUTF8(housing_edit_mode));
    	    house_edit->setText(Wt::WString::fromUTF8(house_edit_mode));
    	    floor_edit->setText(Wt::WString::fromUTF8(house_floor_edit_mode));
    	    apartment_edit->setText(Wt::WString::fromUTF8(apartment_edit_mode));

    }


    dialog->contents()->addStyleClass("form-group");



    /*
    Wt::WRegExpValidator *validator =
        new Wt::WRegExpValidator("[A-Za-z][1-9][0-9]{0,2}");
    validator->setMandatory(true);
    edit->setValidator(validator);
     */
    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);
    //if (wApp->environment().ajax())
     // ok->disable();

    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();

    /*edit->keyWentUp().connect(std::bind([=] () {
	ok->setDisabled(edit->validate() != Wt::WValidator::Valid);
    }));*/

    /*
     * Accept the dialog
     */
    ok->clicked().connect(std::bind([=] () {
	//if (edit->validate())
	    dialog->accept();
    }));

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
		if (operation_name == "create")
		{
			city = city_edit->text().toUTF8();
			district = district_edit->text().toUTF8();
			street = street_edit->text().toUTF8();
			housing = housing_edit->text().toUTF8();
			house_floor = floor_edit->text().toUTF8();
			house = house_edit->text().toUTF8();
			apartment = apartment_edit->text().toUTF8();
			index_number = index_number_edit->text().toUTF8();

			ui->user_address_text_area->setText(Wt::WString::fromUTF8("Город: ") + city_edit->text() + "\n" + Wt::WString::fromUTF8("Район: ") + district_edit->text() + "\n" +
					Wt::WString::fromUTF8("Улица: ") + street_edit->text() + "\n" + Wt::WString::fromUTF8("Корпус: ") + housing_edit->text() + "\n" + Wt::WString::fromUTF8("Этаж:") + floor_edit->text() + "\n" +
					Wt::WString::fromUTF8("Дом: ") + house_edit->text() + "\n" + Wt::WString::fromUTF8("Квартира: ") + apartment_edit->text() + "\n" + Wt::WString::fromUTF8("Индекс: ") + index_number_edit->text());
		} else
		{
			if (city_edit->text() != "") city_edit_mode = city_edit->text().toUTF8();
			if (district_edit->text() != "") district_edit_mode = district_edit->text().toUTF8();
			if (street_edit->text() != "") street_edit_mode = street_edit->text().toUTF8();
			if (housing_edit->text() != "") housing_edit_mode = housing_edit->text().toUTF8();
			if (floor_edit->text() != "") house_floor_edit_mode = floor_edit->text().toUTF8();
			if (house_edit->text() != "") house_edit_mode = house_edit->text().toUTF8();
			if (apartment_edit->text() != "") apartment_edit_mode = apartment_edit->text().toUTF8();
			if (index_number_edit->text() != "") index_number_edit_mode = index_number_edit->text().toUTF8();

			ui->user_address_text_area_edit_user_tab->setText(Wt::WString::fromUTF8("Город: ") + Wt::WString::fromUTF8(city_edit_mode) + "\n" + Wt::WString::fromUTF8("Район: ") + Wt::WString::fromUTF8(district_edit_mode) + "\n" +
					Wt::WString::fromUTF8("Улица: ") + Wt::WString::fromUTF8(street_edit_mode) + "\n" + Wt::WString::fromUTF8("Корпус: ") + Wt::WString::fromUTF8(housing_edit_mode) + "\n" + Wt::WString::fromUTF8("Этаж:") + Wt::WString::fromUTF8(house_floor_edit_mode) + "\n" +
					Wt::WString::fromUTF8("Дом: ") + Wt::WString::fromUTF8(house_edit_mode) + "\n" + Wt::WString::fromUTF8("Квартира: ") + Wt::WString::fromUTF8(apartment_edit_mode) + "\n" + Wt::WString::fromUTF8("Индекс: ") + Wt::WString::fromUTF8(index_number_edit_mode));
		}

	}

	delete dialog;
    }));

    dialog->show();
}





// subscriber services &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

// show dialog window where you add subscriber services
extern void WtAccounts::add_service_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Абонентская услуга"));

    dialog->resize(600, 750);

	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");



// dialog contents
    Wt::WTreeTable *treeTable;
	treeTable = new Wt::WTreeTable(dialog->contents());
    treeTable->resize(550, 300);
    //treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
    treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable"));
    treeTable->tree()->setSelectionMode(Wt::SingleSelection);
    //treeTable->addColumn(Wt::WString::fromUTF8("Абонентов"), 100);


    Wt::WTreeTableNode *tree_root;
	tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("Абонентские услуги"));
    treeTable->setTreeRoot(tree_root, Wt::WString::fromUTF8("Названия"));

    Wt::WTreeTableNode *tree_node;



// add node form mysql table
	query_state=mysql_query(conn, "select * from subscriber_services");
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tree_node  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[1]), 0, tree_root);
		//tree_node ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(row[2])));
	}


	mysql_free_result(res);

	tree_root->expand();



    // quantity label and edit
    Wt::WLabel *amount_label = new Wt::WLabel(Wt::WString::fromUTF8("Количество:"),
    				       dialog->contents());
    amount_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *amount_edit_container = new Wt::WContainerWidget(dialog->contents());
    amount_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    amount_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *amount_edit = new Wt::WLineEdit(amount_edit_container);
    amount_edit->setText(Wt::WString::fromUTF8(""));
    amount_label->setBuddy(amount_edit);



    // number/ip label and combo box
    Wt::WLabel *number_ip_label = new Wt::WLabel(Wt::WString::fromUTF8("Номер/IP:"),
    				       dialog->contents());
    number_ip_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *number_ip_combobox_container = new Wt::WContainerWidget(dialog->contents());
    number_ip_combobox_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    number_ip_combobox_container->setHtmlTagName("div");

    Wt::WComboBox *number_ip_combobox = new Wt::WComboBox(number_ip_combobox_container);
    number_ip_label->setBuddy(number_ip_combobox);

    std::string mysql_get_number_ip = "";
    if (operation_name == "create") {
    	mysql_get_number_ip = "SELECT ip_address FROM ip_addresses "
    			"WHERE ip_addresses.ip_status = 'not_processed'";
		query_state=mysql_query(conn, mysql_get_number_ip.c_str());

		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			number_ip_combobox->addItem(Wt::WString::fromUTF8(row[0]));
		}

		mysql_get_number_ip = "SELECT number FROM phone_numbers "
				"WHERE phone_numbers.number_status = 'not_processed'";
		query_state=mysql_query(conn, mysql_get_number_ip.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			number_ip_combobox->addItem(Wt::WString::fromUTF8(row[0]));
		}
    } else { // operation_name == "edit"

    	mysql_get_number_ip = "SELECT ip_address FROM ip_addresses "
    			"WHERE ip_addresses.subscriber_id = '"+ subscriber_id_edit_mode +"'";
		query_state=mysql_query(conn, mysql_get_number_ip.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			number_ip_combobox->addItem(Wt::WString::fromUTF8(row[0]));
		}

    	mysql_get_number_ip = "SELECT number FROM phone_numbers "
    			"WHERE phone_numbers.subscriber_id = '"+ subscriber_id_edit_mode +"'";
		query_state=mysql_query(conn, mysql_get_number_ip.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			number_ip_combobox->addItem(Wt::WString::fromUTF8(row[0]));
		}
    }



    // description label and text area
    Wt::WLabel *description_label = new Wt::WLabel(Wt::WString::fromUTF8("Описание:"),
    				       dialog->contents());
    description_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *description_text_area_container = new Wt::WContainerWidget(dialog->contents());
    description_text_area_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    description_text_area_container->setHtmlTagName("div");

    Wt::WTextArea *description_text_area = new Wt::WTextArea(description_text_area_container);
    description_text_area->setColumns(80);
    description_text_area->setRows(3);
    description_label->setBuddy(description_text_area);



    // include date label, check box and date edit
    Wt::WLabel *include_date_label = new Wt::WLabel(Wt::WString::fromUTF8("С:"),
    				       dialog->contents());
    include_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *include_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    include_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *include_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    include_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    include_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *include_date_edit = new Wt::WDateEdit(include_date_edit_container);
    include_date_edit->setFormat("yyyy-MM-dd");
    include_date_label->setBuddy(include_date_edit);



    // disable date label, check box and date edit
    Wt::WLabel *disable_date_label = new Wt::WLabel(Wt::WString::fromUTF8("По:"),
    				       dialog->contents());
    disable_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *disable_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    disable_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *disable_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    disable_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    disable_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *disable_date_edit = new Wt::WDateEdit(disable_date_edit_container);
    disable_date_edit->setFormat("yyyy-MM-dd");
    disable_date_label->setBuddy(disable_date_edit);



	mysql_free_result(res);
	mysql_close(conn);



    dialog->contents()->addStyleClass("form-group");


    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{

		std::string service_name = "";
		std::string amount = "";
		std::string number_ip = "";
		std::string description = "";
		std::string date_since = "";
		std::string date_by = "";



		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}
			service_name = selected_node->label()->text().toUTF8();
			highlightedRows.clear();
		}



		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");



		amount = amount_edit->text().toUTF8();
		number_ip = number_ip_combobox->currentText().toUTF8();
		description = description_text_area->text().toUTF8();

		if (include_date_checkbox->isChecked())
		{
			date_since= boost::lexical_cast<std::string>(include_date_edit->text());
		}

		if (disable_date_checkbox->isChecked())
		{
			date_by = boost::lexical_cast<std::string>(disable_date_edit->text());
		}

		std::string mysql_insert_add_service = "";



		if (operation_name == "create")
		{
			mysql_insert_add_service = "INSERT INTO subscriber_connected_services "
					"(subscriber_id, service_id, service, quantity, number_ip, "
					"description, date_since, date_by, service_status, charging_period) "
					"values (NULL, NULL, '"+ service_name +"', '"+ amount +"', "
					"'"+ number_ip +"', '"+ description +"', '"+ date_since +"', "
					"'"+ date_by +"', 'not_processed', '')";
		} else // operation_name == "edit"
		{
			mysql_insert_add_service =  "INSERT INTO subscriber_connected_services "
					"(subscriber_id, service_id, service, quantity, number_ip, "
					"description, date_since, date_by, service_status, charging_period)  "
					"values ('"+subscriber_id_edit_mode+"', NULL, '"+ service_name +"', "
					"'"+ amount +"', '"+ number_ip +"', '"+ description +"', "
					"'"+ date_since +"', '"+ date_by +"', 'processed', '')";
		}

		query_state=mysql_query(conn, mysql_insert_add_service.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}


		subscriber_connected_table->clear();
		subscriber_connected_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Услуга")));
		subscriber_connected_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер/IP")));
		subscriber_connected_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
		subscriber_connected_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
		subscriber_connected_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Количество")));
		subscriber_connected_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

		int row_number = 0;



		std::string mysql_service_table_data = "";

		if (operation_name == "create")
		{
			mysql_service_table_data = "select service, quantity, number_ip, description, date_since, date_by "
					  "from subscriber_connected_services where subscriber_connected_services.service_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_service_table_data = "select service, quantity, number_ip, description, date_since, date_by "
					  "from subscriber_connected_services where subscriber_connected_services.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}

		query_state=mysql_query(conn, mysql_service_table_data.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[0]), subscriber_connected_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), subscriber_connected_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), subscriber_connected_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), subscriber_connected_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[1]), subscriber_connected_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), subscriber_connected_table->elementAt(row_number, 5));
		}
		subscriber_connected_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// show dialog window where you delete subscriber services
extern void WtAccounts::delete_service_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Удалить Абонентскую Услугу"));

    dialog->resize(600, 550);



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	/*
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	 */
	mysql_query(&mysql,"SET NAMES 'UTF8'");



    Wt::WTreeTable *treeTable;
	treeTable = new Wt::WTreeTable(dialog->contents());
    //treeTable->resize(650, 300);
    treeTable->setStyleClass(Wt::WString::fromUTF8("Wt-treetable col-lg-12 col-md-12"));
    treeTable->tree()->setSelectionMode(Wt::SingleSelection);
    treeTable->addColumn(Wt::WString::fromUTF8("Номер/IP"), 100);
    //treeTable->addColumn(Wt::WString::fromUTF8("Фикс. сумма"), 100);

    Wt::WTreeTableNode *tree_root;
	tree_root = new Wt::WTreeTableNode(Wt::WString::fromUTF8("Абонентские услуги"));
    treeTable->setTreeRoot(tree_root, Wt::WString::fromUTF8("Названия"));

    Wt::WTreeTableNode *tree_node;

    std::string mysql_get_ip_addresses = "";

// add node form mysql table
	if (operation_name == "create")
	{
		mysql_get_ip_addresses = "SELECT service, number_ip FROM subscriber_connected_services "
				"WHERE service_status = 'not_processed'";
	} else // operation_name == "edit"
	{
		mysql_get_ip_addresses = "SELECT service, number_ip FROM subscriber_connected_services "
				"WHERE  subscriber_id = '"+subscriber_id_edit_mode+"'";
	}
	query_state=mysql_query(conn, mysql_get_ip_addresses.c_str());
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
	while((row=mysql_fetch_row(res))!=NULL)
	{
		tree_node  = new Wt::WTreeTableNode(Wt::WString::fromUTF8(row[0]), 0, tree_root);
		tree_node ->setColumnWidget(1, new Wt::WText(Wt::WString::fromUTF8(row[1])));
		//tree_node ->setColumnWidget(2, new Wt::WText(Wt::WString::fromUTF8(row[2])));
	}

	mysql_free_result(res);
	mysql_close(conn);

	tree_root->expand();



    Wt::WPushButton *ok = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{


		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		/*
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		 */
		mysql_query(&mysql,"SET NAMES 'UTF8'");



		std::string mysql_delete_ip_address = "";
		std::string selected_service = "";

		Wt::WTreeNode *selected_node; // operator* returns contents of an interator
		std::set<Wt::WTreeNode* > highlightedRows = treeTable->tree()->selectedNodes();
		if (!highlightedRows.empty())
		{
			for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
			{
			  selected_node = *i;
			}

			selected_service= selected_node->label()->text().toUTF8();

			if (operation_name == "create")
			{
				mysql_delete_ip_address = "DELETE FROM subscriber_connected_services "
						"WHERE service = '"+selected_service+"'";
			} else // operation_name == "edit"
			{
				mysql_delete_ip_address = "DELETE FROM subscriber_connected_services "
						"WHERE service = '"+selected_service+"' "
						"AND subscriber_id = '"+subscriber_id_edit_mode+"'";
			}
			highlightedRows.clear();
		}


		query_state=mysql_query(conn, mysql_delete_ip_address.c_str());

		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}



		subscriber_connected_table->clear();
		subscriber_connected_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Услуга")));
		subscriber_connected_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер/IP")));
		subscriber_connected_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
		subscriber_connected_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
		subscriber_connected_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Количество")));
		subscriber_connected_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

		int row_number = 0;



		std::string mysql_service_table_data = "";

		if (operation_name == "create")
		{
			mysql_service_table_data = "select service, quantity, number_ip, description, date_since, date_by "
					  "from subscriber_connected_services where subscriber_connected_services.service_status = 'not_processed'";
		} else // operation_name == "edit"
		{
			mysql_service_table_data = "select service, quantity, number_ip, description, date_since, date_by "
					  "from subscriber_connected_services where subscriber_connected_services.subscriber_id = '"+subscriber_id_edit_mode+"'";
		}

		query_state=mysql_query(conn, mysql_service_table_data.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[0]), subscriber_connected_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), subscriber_connected_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), subscriber_connected_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), subscriber_connected_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[1]), subscriber_connected_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), subscriber_connected_table->elementAt(row_number, 5));
		}
		subscriber_connected_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}





// !!!!!!!!! important !!!!!!!!!
// in create tab for button save and exit write code that handle 'not_processed' entries
// show dialog window where you manage (add, delete etc) subscriber services
extern void WtAccounts::subscriber_service_dialog(std::string operation_name)
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Абонентские Услуги"));

    dialog->resize(1050, 450);



    // srevice top buttons
    Wt::WPushButton *save_exit_button = new Wt::WPushButton(Wt::WString::fromUTF8("Сохранить и закрыть"), dialog->contents());
    save_exit_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    Wt::WPushButton *add_service_button = new Wt::WPushButton(Wt::WString::fromUTF8("Добавить"), dialog->contents());
    add_service_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    //Wt::WPushButton *change_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Изменить"), dialog->contents());
    //change_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    Wt::WPushButton *delete_service_button = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->contents());
    delete_service_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));


	add_service_button->clicked().connect(boost::bind(&WtAccounts::add_service_dialog, this, operation_name));
	delete_service_button->clicked().connect(boost::bind(&WtAccounts::delete_service_dialog, this, operation_name));



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");




	// table with data about ip address
	subscriber_connected_table = new Wt::WTable(dialog->contents());
	subscriber_connected_table->setHeaderCount(1);
	subscriber_connected_table->setWidth(Wt::WLength("100%"));

	subscriber_connected_table->setHeaderCount(1);

	subscriber_connected_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Услуга")));
	subscriber_connected_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Номер/IP")));
	subscriber_connected_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("С")));
	subscriber_connected_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("По")));
	subscriber_connected_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Количество")));
	subscriber_connected_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Описание")));

	int row_number = 0;



	std::string mysql_subscriber_service_table_data = "";

	if (operation_name == "create")
	{
		mysql_subscriber_service_table_data = "SELECT service, quantity, number_ip, description, date_since, date_by "
				  "FROM subscriber_connected_services WHERE service_status = 'not_processed'";
	} else // operation_name == "edit"
	{
		mysql_subscriber_service_table_data = "SELECT service, quantity, number_ip, description, date_since, date_by "
				  "FROM subscriber_connected_services WHERE subscriber_id = '"+ subscriber_id_edit_mode +"'";
	}

	query_state=mysql_query(conn, mysql_subscriber_service_table_data.c_str());
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		row_number++;
		new Wt::WText(Wt::WString::fromUTF8(row[0]), subscriber_connected_table->elementAt(row_number, 0));
		new Wt::WText(Wt::WString::fromUTF8(row[2]), subscriber_connected_table->elementAt(row_number, 1));
		new Wt::WText(Wt::WString::fromUTF8(row[4]), subscriber_connected_table->elementAt(row_number, 2));
		new Wt::WText(Wt::WString::fromUTF8(row[5]), subscriber_connected_table->elementAt(row_number, 3));
		new Wt::WText(Wt::WString::fromUTF8(row[1]), subscriber_connected_table->elementAt(row_number, 4));
		new Wt::WText(Wt::WString::fromUTF8(row[3]), subscriber_connected_table->elementAt(row_number, 5));
	}
	row_number = 0;
	subscriber_connected_table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped top_mar top_pad");


	mysql_free_result(res);
	mysql_close(conn);;



    dialog->contents()->addStyleClass("form-group");



    //Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    //ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    save_exit_button->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
	}
	else
	{
	}

	delete dialog;
    }));

    dialog->show();
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&





// tabs' top button functions
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^





// save data from all entries in tab (operation_name determine what tab it's) and close this tab





extern void WtAccounts::save_data_and_close_tab(std::string operation_name)
{
	if (operation_name == "create")
	{
		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
			std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");
		if(conn==NULL)
		{
			std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}

// get group id through group name
		if(!group_name.empty())
		{
			mysql_query_str = "select group_id from subscriber_group where group_name = '"+group_name+"'";
			query_state=mysql_query(conn, mysql_query_str.c_str());
			if(query_state!=0)
			{
			   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			}
			res=mysql_store_result(conn);
			std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
			row=mysql_fetch_row(res);
			group_id = boost::lexical_cast<std::string>(row[0]);


					mysql_query_str = "UPDATE subscriber_group SET subscribers_number_ingroup = subscribers_number_ingroup + 1 WHERE group_id = '"+group_id+"'";
					query_state=mysql_query(conn, mysql_query_str.c_str());
					if(query_state!=0)
					{
					   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
					}
		}


	// get main subscriber info and write in mysql table
		//group_id = "1";
		//Wt::WString temp =ui->user_full_name_edit->text().toUTF8();
		full_name = ui->user_full_name_edit->text().toUTF8();
		//full_name = boost::locale::conv::utf_to_utf<std::string>(ui->user_full_name_edit->text());
		subscriber_type = ui->user_type_combo_box->currentText().toUTF8();
		account_number = ui->account_number_edit->text().narrow();
		user_login = ui->user_login_edit->text().toUTF8();
		user_password = ui->user_pass_edit->text().toUTF8();
		tariff_plan = ui->tariff_plan_edit->text().toUTF8();
		user_email = ui->user_email_edit->text().narrow();

		if (ui->user_include_date_checkbox->isChecked())
		{
			ui->user_include_date_date_edit->setFormat("yyyy-MM-dd");
			date_since = boost::lexical_cast<std::string>(ui->user_include_date_date_edit->text());
		}


		if (ui->user_disable_date_checkbox->isChecked())
		{
			ui->user_disable_date_date_edit->setFormat("yyyy-MM-dd");
			date_by = boost::lexical_cast<std::string>(ui->user_disable_date_date_edit->text());
		}



		std::cout << "Adilbek Cool" << "***************************************************" << std::endl;



		std::string mysql_subscriber = "INSERT INTO subscriber (group_id, name, surname, patronymic, "
				"full_name, subscriber_type, account_number, user_login, user_password,"
				"date_since, date_by, tariff_plan, user_email) "
				"values ('"+group_id+"', '"+name+"','"+surname+"', '"+patronymic+"',"
				"'"+full_name+"', '"+subscriber_type+"','"+account_number+"', '"+user_login+"',"
				"'"+user_password+"', '"+date_since+"','"+date_by+"', '"+tariff_plan+"','"+user_email+"')";
		query_state=mysql_query(conn, mysql_subscriber.c_str());

		// get new subscriber id
		query_state=mysql_query(conn, "select LAST_INSERT_ID() from subscriber");

		res=mysql_store_result(conn);
		std::cout<<"Last insert ID in the amaDB table."<<std::endl<<std::endl;
		//row=mysql_fetch_row(res);
		//std::cout<<mysql_fetch_row(res)[0]<<std::endl<<std::endl;
		subscriber_id = boost::lexical_cast<std::string>(mysql_fetch_row(res)[0]);


		//std::cout << "Adilbek Cool " << subscriber_id << std::endl;
		//std::cout << "Adilbek Cool" << "***************************************************" << std::endl;

		// update subscriber_id in ip_addresses table (it's because when we create ip we don't now id of current subscriber) and change ip_satus on "processed"
		std::string mysql_ip_adreess_subscriber_id = "UPDATE ip_addresses SET subscriber_id = '"+subscriber_id+"', ip_status = 'processed' WHERE ip_addresses.ip_status = 'not_processed'";
		query_state=mysql_query(conn, mysql_ip_adreess_subscriber_id.c_str());

		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}



		// update subscriber_id in phone_numbers table (it's because when we create phone number we don't now id of current subscriber) and change number_satus on "processed"
		std::string mysql_phone_number_subscriber_id = "UPDATE phone_numbers SET subscriber_id = '"+subscriber_id+"', number_status = 'processed' WHERE phone_numbers.number_status = 'not_processed'";
		query_state=mysql_query(conn, mysql_phone_number_subscriber_id.c_str());

		if(query_state!=0)
		{
			  std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}



		// update subscriber_id in subscriber_connected_services table (it's because when we add service to subscriber we don't now id of current subscriber) and change service_status on "processed"
		std::string mysql_subscriber_service_subscriber_id = "UPDATE subscriber_connected_services "
				"SET subscriber_id = '"+subscriber_id+"', service_status = 'processed' "
				"WHERE subscriber_connected_services.service_status = 'not_processed'";
		query_state=mysql_query(conn, mysql_subscriber_service_subscriber_id.c_str());

		if(query_state!=0)
		{
			  std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}



		// subscriber contacts data write in mysql table
		contact_numbers = ui->contact_number_edit->text().toUTF8();

		std::string mysql_subscriber_contacts = "INSERT INTO contacts (subscriber_id, city, district, "
				"street, housing, floor, house, apartment, index_number, contact_numbers) "
				"values ('"+subscriber_id+"', '"+city+"', '"+district+"','"+street+"', '"+housing+"','"+house_floor+"', "
						"'"+house+"','"+apartment+"', '"+index_number+"','"+contact_numbers+"')";
		query_state=mysql_query(conn, mysql_subscriber_contacts.c_str());








//get subscriber requisites and information fields data and write in mysql tables

		// get subscriber requisites data
		personal_code = ui->personal_code_edit->text().toUTF8();
		organization_code = ui->organization_code_edit->text().toUTF8();
		tax_code =  ui->tax_code_edit->text().toUTF8();
		account =  ui->requisite_account_edit->text().toUTF8();
		bank_code =  ui->bank_code_edit->text().toUTF8();
		agreement =  ui->agreement_edit->text().toUTF8();

		if (ui->agreement_checkbox->isChecked())
		{
			ui->agreement_date_edit->setFormat("yyyy-MM-dd");
			agreement_date = ui->agreement_date_edit->text().narrow(); // may occur error narrow()
		}

		// write requisites in mysql table
		std::string mysql_subscriber_requisites = "INSERT INTO requisites (subscriber_id, personal_code, "
				"organization_code, tax_code, account, bank_code, agreement, agreement_date) "
				"values ('"+subscriber_id+"', '"+personal_code+"', '"+organization_code+"', '"+tax_code+"', "
						"'"+account+"', '"+bank_code+"', '"+agreement+"', '"+agreement_date+"')";
		query_state=mysql_query(conn, mysql_subscriber_requisites.c_str());



		// get subscriber information fields data
		description = ui->description_text_area->text().toUTF8();
		information = ui->information_text_area->text().toUTF8();
		//subscriber_id = "1";
		// write information fields in mysql table
		std::string mysql_subscriber_information_fields = "INSERT INTO information_fields (subscriber_id, description, "
				"information) values ('"+subscriber_id+"', '"+description+"', '"+information+"')";
		query_state=mysql_query(conn, mysql_subscriber_information_fields.c_str());



	// clear all data variables

		// subscriber group info variable
		std::string group_name = "";

		// subscriber contacts data variables
		subscriber_id = "";
		group_id = "";
		name = "";
		surname = "";
		patronymic = "";
		full_name = "";
		subscriber_type = "";
		account_number = "";
		user_login = "";
		user_password = "";
		date_since = "";
		date_by = "";
		tariff_plan  = "";
		user_email = "";

		// subscriber contacts data variables
		city = "";
		district = "";
		street = "";
		housing = "";
		house_floor = "";
		house = "";
		apartment = "";
		index_number = "";
		contact_numbers = "";

		// subscriber requisites data variables
		personal_code = "";
		organization_code = "";
		tax_code = "";
		account = "";
		bank_code = "";
		agreement = "";
		agreement_date = "";

		// subscriber information fields data variables
		description = "";
		information = "";

		// close(hide) this tab
		ui->create_user_tab_mi->setHidden(true);
		ui->main_tabs->setCurrentIndex(0);

		mysql_free_result(res);
		mysql_close(conn);
	} else // operation_name == "create" ############################################################################
	{
		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
			std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");
		if(conn==NULL)
		{
			std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}


// get group id through group name
		if(!new_group_name_edit_mode.empty())
		{
			std::string old_group_id_edit_mode = group_id_edit_mode; // saved in variable group id from mysql table
			mysql_query_str = "select group_id from subscriber_group where group_name = '"+new_group_name_edit_mode+"'";
			query_state=mysql_query(conn, mysql_query_str.c_str());
			if(query_state!=0)
			{
			   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
			}
			res=mysql_store_result(conn);
			std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
			row=mysql_fetch_row(res);
			group_id_edit_mode = boost::lexical_cast<std::string>(row[0]); // get group id for new group name

			// if subscriber group changed decrease subscribers_number_ingroup in old group and increase in new group
			if (new_group_name_edit_mode != group_name_edit_mode) {
				mysql_query_str = "UPDATE subscriber_group SET subscribers_number_ingroup = subscribers_number_ingroup - 1 WHERE group_id = '"+old_group_id_edit_mode+"'";
				query_state=mysql_query(conn, mysql_query_str.c_str());
				if(query_state!=0)
				{
					std::cout<<mysql_error(conn)<<std::endl<<std::endl;
				}

				mysql_query_str = "UPDATE subscriber_group SET subscribers_number_ingroup = subscribers_number_ingroup + 1 WHERE group_id = '"+group_id_edit_mode+"'";
				query_state=mysql_query(conn, mysql_query_str.c_str());
				if(query_state!=0)
				{
					std::cout<<mysql_error(conn)<<std::endl<<std::endl;
				}
			}
		}



// get info from some form entry where not updates value automatically(i.e when dialog window close value in some variable change this variable I'm not change)
			// get main subscriber info
			full_name_edit_mode = ui->user_full_name_edit_edit_user_tab->text().toUTF8();
			subscriber_type_edit_mode = ui->user_type_combo_box_edit_user_tab->currentText().toUTF8();
			account_number_edit_mode = ui->account_number_edit_edit_user_tab->text().narrow();
			user_login_edit_mode = ui->user_login_edit_edit_user_tab->text().toUTF8();
			user_password_edit_mode = ui->user_pass_edit_edit_user_tab->text().toUTF8();
			tariff_plan_edit_mode = ui->tariff_plan_edit_edit_user_tab->text().toUTF8();
			user_email_edit_mode = ui->user_email_edit_edit_user_tab->text().narrow();

			if (ui->user_include_date_checkbox_edit_user_tab->isChecked())
			{
				ui->user_include_date_date_edit_edit_user_tab->setFormat("yyyy-MM-dd");
				date_since_edit_mode = boost::lexical_cast<std::string>(ui->user_include_date_date_edit_edit_user_tab->text());
			}
			else
			{
				date_since_edit_mode = "";
			}


			if (ui->user_disable_date_checkbox_edit_user_tab->isChecked())
			{
				ui->user_disable_date_date_edit_edit_user_tab->setFormat("yyyy-MM-dd");
				date_by_edit_mode = boost::lexical_cast<std::string>(ui->user_disable_date_date_edit_edit_user_tab->text());
			}
			else
			{
				date_by_edit_mode = "";
			}

			// get subscriber contact numbers
			contact_numbers_edit_mode = ui->contact_number_edit_edit_user_tab->text().narrow();

			// get subscriber requisites data
			personal_code_edit_mode = ui->personal_code_edit_edit_user_tab->text().toUTF8();
			organization_code_edit_mode = ui->organization_code_edit_edit_user_tab->text().toUTF8();
			tax_code_edit_mode =  ui->tax_code_edit_edit_user_tab->text().toUTF8();
			account_edit_mode =  ui->requisite_account_edit_edit_user_tab->text().toUTF8();
			bank_code_edit_mode =  ui->bank_code_edit_edit_user_tab->text().toUTF8();
			agreement_edit_mode =  ui->agreement_edit_edit_user_tab->text().toUTF8();

			if (ui->agreement_checkbox_edit_user_tab->isChecked())
			{
				ui->agreement_date_edit_edit_user_tab->setFormat("yyyy-MM-dd");
				agreement_date_edit_mode = ui->agreement_date_edit_edit_user_tab->text().narrow(); // may occur error narrow()
			}
			else
			{
				agreement_date_edit_mode = "";
			}


			// get subscriber information fields data
			description_edit_mode = ui->description_text_area_edit_user_tab->text().toUTF8();
			information_edit_mode = ui->information_text_area_edit_user_tab->text().toUTF8();



		// update all subscriber information
		std::string mysql_subscriber_update_all_data = "UPDATE subscriber_group, subscriber, contacts, requisites, information_fields "
						"SET subscriber.group_id = '"+group_id_edit_mode+"', name = '"+name_edit_mode+"', surname = '"+surname_edit_mode+"', "
						"patronymic = '"+patronymic_edit_mode+"', full_name = '"+full_name_edit_mode+"', subscriber_type = '"+subscriber_type_edit_mode+"', "
						"account_number = '"+account_number_edit_mode+"', user_login = '"+user_login_edit_mode+"', user_password = '"+user_password_edit_mode+"', "
						"date_since = '"+date_since_edit_mode+"', date_by = '"+date_by_edit_mode+"', tariff_plan = '"+tariff_plan_edit_mode+"', "
						"user_email = '"+user_email_edit_mode+"', city = '"+city_edit_mode+"', district = '"+district_edit_mode+"', street = '"+street_edit_mode+"', "
						"housing = '"+housing_edit_mode+"', floor = '"+house_floor_edit_mode+"', house = '"+house_edit_mode+"', apartment = '"+apartment_edit_mode+"', "
						"index_number = '"+index_number_edit_mode+"', contact_numbers = '"+contact_numbers_edit_mode+"', personal_code = '"+personal_code_edit_mode+"', "
						"organization_code = '"+organization_code_edit_mode+"', tax_code = '"+tax_code_edit_mode+"', account = '"+account_edit_mode+"', "
						"bank_code = '"+bank_code_edit_mode+"', agreement = '"+agreement_edit_mode+"', agreement_date = '"+agreement_date_edit_mode+"', "
						"description = '"+description_edit_mode+"', information = '"+information_edit_mode+"' "
						"WHERE subscriber.subscriber_id = '"+subscriber_id_edit_mode+"' AND subscriber_group.group_id = subscriber.group_id "
						"AND subscriber.subscriber_id = contacts.subscriber_id AND contacts.subscriber_id = requisites.subscriber_id "
						"AND requisites.subscriber_id = information_fields.subscriber_id";

		query_state=mysql_query(conn, mysql_subscriber_update_all_data.c_str());



// clear all data variables

		// subscriber group info variable
		group_name_edit_mode = "";

		// subscriber contacts data variables
		subscriber_id_edit_mode = "";
		group_id_edit_mode = "";
		name_edit_mode = "";
		surname_edit_mode = "";
		patronymic_edit_mode = "";
		full_name_edit_mode = "";
		subscriber_type_edit_mode = "";
		account_number_edit_mode = "";
		user_login_edit_mode = "";
		user_password_edit_mode = "";
		date_since_edit_mode = "";
		date_by_edit_mode = "";
		tariff_plan_edit_mode  = "";
		user_email_edit_mode = "";

		// subscriber contacts data variables
		city_edit_mode = "";
		district_edit_mode = "";
		street_edit_mode = "";
		housing_edit_mode = "";
		house_floor_edit_mode = "";
		house_edit_mode = "";
		apartment_edit_mode = "";
		index_number_edit_mode = "";
		contact_numbers_edit_mode = "";

		// subscriber requisites data variables
		personal_code_edit_mode = "";
		organization_code_edit_mode = "";
		tax_code_edit_mode = "";
		account_edit_mode = "";
		bank_code_edit_mode = "";
		agreement_edit_mode = "";
		agreement_date_edit_mode = "";

		// subscriber information fields data variables
		description_edit_mode = "";
		information_edit_mode = "";

		// close(hide) this tab
		ui->edit_user_tab_mi->setHidden(true);
		ui->main_tabs->setCurrentIndex(0);

		mysql_free_result(res);
		mysql_close(conn);
	}

}



// services %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%








// show dialog window where you can create new services
extern void WtAccounts::create_new_service_dialog()
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Услуги"));

    dialog->resize(600, 750);



	// dialog contents
    // service name label and edit
    Wt::WLabel *service_name_label = new Wt::WLabel(Wt::WString::fromUTF8("Название:"),
    				       dialog->contents());
    service_name_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *service_name_edit_container = new Wt::WContainerWidget(dialog->contents());
    service_name_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    service_name_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *service_name_edit = new Wt::WLineEdit(service_name_edit_container);
    service_name_edit->setText(Wt::WString::fromUTF8(""));
    service_name_label->setBuddy(service_name_edit);



    // code label and edit
    Wt::WLabel *code_label = new Wt::WLabel(Wt::WString::fromUTF8("Код:"),
    				       dialog->contents());
    code_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *code_edit_container = new Wt::WContainerWidget(dialog->contents());
    code_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    code_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *code_edit = new Wt::WLineEdit(code_edit_container);
    code_edit->setText(Wt::WString::fromUTF8(""));
    code_label->setBuddy(code_edit);



    // charging period label and combo box
    Wt::WLabel *charging_period_label = new Wt::WLabel(Wt::WString::fromUTF8("Период начисления:"),
    				       dialog->contents());
    charging_period_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *charging_period_combobox_container = new Wt::WContainerWidget(dialog->contents());
    charging_period_combobox_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    charging_period_combobox_container->setHtmlTagName("div");

    Wt::WComboBox *charging_period_combobox = new Wt::WComboBox(charging_period_combobox_container);
    charging_period_label->setBuddy(charging_period_combobox);


	charging_period_combobox->addItem(Wt::WString::fromUTF8("Ручное начисление"));
	charging_period_combobox->addItem(Wt::WString::fromUTF8("Каждый день"));
	charging_period_combobox->addItem(Wt::WString::fromUTF8("Каждую неделю"));
	charging_period_combobox->addItem(Wt::WString::fromUTF8("Каждый месяц"));
	charging_period_combobox->addItem(Wt::WString::fromUTF8("Каждый год"));
	charging_period_combobox->addItem(Wt::WString::fromUTF8("Особый сценарий"));
	charging_period_combobox->addItem(Wt::WString::fromUTF8("Авто"));



    // description label and text area
    Wt::WLabel *description_label = new Wt::WLabel(Wt::WString::fromUTF8("Описание:"),
    				       dialog->contents());
    description_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *description_text_area_container = new Wt::WContainerWidget(dialog->contents());
    description_text_area_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    description_text_area_container->setHtmlTagName("div");

    Wt::WTextArea *description_text_area = new Wt::WTextArea(description_text_area_container);
    description_text_area->setColumns(80);
    description_text_area->setRows(3);
    description_label->setBuddy(description_text_area);



    // service tariff label and edit
    Wt::WLabel *service_tariff_label = new Wt::WLabel(Wt::WString::fromUTF8("Тариф:"),
    				       dialog->contents());
    service_tariff_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *service_tariff_edit_container = new Wt::WContainerWidget(dialog->contents());
    service_tariff_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    service_tariff_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *service_tariff_edit = new Wt::WLineEdit(service_tariff_edit_container);
    service_tariff_edit->setText(Wt::WString::fromUTF8("Фиксированная  сумма"));
    service_tariff_label->setBuddy(service_tariff_edit);



    // fixed sum label and edit
    Wt::WLabel *fixed_sum_label = new Wt::WLabel(Wt::WString::fromUTF8("Фикс. сумма:"),
    				       dialog->contents());
    fixed_sum_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *fixed_sum_edit_container = new Wt::WContainerWidget(dialog->contents());
    fixed_sum_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    fixed_sum_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *fixed_sum_edit = new Wt::WLineEdit(fixed_sum_edit_container);
    fixed_sum_edit->setText(Wt::WString::fromUTF8(""));
    fixed_sum_label->setBuddy(fixed_sum_edit);



    // currency label and edit
    Wt::WLabel *currency_label = new Wt::WLabel(Wt::WString::fromUTF8("Валюта:"),
    				       dialog->contents());
    currency_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");

    Wt::WContainerWidget *currency_edit_container = new Wt::WContainerWidget(dialog->contents());
    currency_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-9 col-lg-9 top_mar"));
    currency_edit_container->setHtmlTagName("div");

    Wt::WLineEdit *currency_edit = new Wt::WLineEdit(currency_edit_container);
    currency_edit->setText(Wt::WString::fromUTF8("KZT (Тенге)"));
    currency_label->setBuddy(currency_edit);



/*
    // include date label, check box and date edit
    Wt::WLabel *include_date_label = new Wt::WLabel(Wt::WString::fromUTF8("С:"),
    				       dialog->contents());
    include_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *include_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    include_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *include_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    include_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    include_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *include_date_edit = new Wt::WDateEdit(include_date_edit_container);
    include_date_edit->setFormat("yyyy-MM-dd");
    include_date_label->setBuddy(include_date_edit);



    // disable date label, check box and date edit
    Wt::WLabel *disable_date_label = new Wt::WLabel(Wt::WString::fromUTF8("По:"),
    				       dialog->contents());
    disable_date_label->setStyleClass("col-lg-3 col-md-3 top_pad top_mar");


    Wt::WCheckBox *disable_date_checkbox = new Wt::WCheckBox("", dialog->contents());
    disable_date_checkbox->setStyleClass("col-lg-1 col-md-1 top_pad");

    Wt::WContainerWidget *disable_date_edit_container = new Wt::WContainerWidget(dialog->contents());
    disable_date_edit_container->setStyleClass(Wt::WString::fromUTF8("col-md-8 col-lg-8 top_mar"));
    disable_date_edit_container->setHtmlTagName("div");

    Wt::WDateEdit *disable_date_edit = new Wt::WDateEdit(disable_date_edit_container);
    disable_date_edit->setFormat("yyyy-MM-dd");
    disable_date_label->setBuddy(disable_date_edit);
*/



    dialog->contents()->addStyleClass("form-group");


    Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    ok->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{

		std::string service_name = "";
		std::string code = "";
		std::string charging_period = "";
		std::string description = "";
		std::string service_tariff = "";
		std::string fixed_sum = "";
		std::string currency = "";

		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");



		service_name = service_name_edit->text().toUTF8();;
		code = code_edit->text().toUTF8();;
		charging_period = charging_period_combobox->currentText().toUTF8();
		description = description_text_area->text().toUTF8();;
		service_tariff = service_tariff_edit->text().toUTF8();;
		fixed_sum = fixed_sum_edit->text().toUTF8();;
		currency = currency_edit->text().toUTF8();;

		std::string mysql_insert_create_service = "INSERT INTO subscriber_services "
					"(service_name, code, charging_period, description, service_tariff, "
					"amount, currency) "
					"values ('"+ service_name +"', '"+ code +"', '"+ charging_period +"', "
					"'"+ description +"', '"+ service_tariff +"', '"+ fixed_sum +"',  "
					"'"+ currency +"')";


		query_state=mysql_query(conn, mysql_insert_create_service.c_str());
		if(query_state!=0)
		{
			std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}


		service_table->clear();
		service_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Название")));
		service_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Период начисления")));
		service_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Код")));
		service_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тариф")));
		service_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Фикс. сумма")));
		service_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Валюта")));

		int row_number = 0;



		std::string mysql_service_table_data = "SELECT service_name, code, charging_period, "
					"service_tariff, amount, currency FROM subscriber_services";

		query_state=mysql_query(conn, mysql_service_table_data.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		while((row=mysql_fetch_row(res))!=NULL)
		{
			row_number++;
			new Wt::WText(Wt::WString::fromUTF8(row[0]), service_table->elementAt(row_number, 0));
			new Wt::WText(Wt::WString::fromUTF8(row[2]), service_table->elementAt(row_number, 1));
			new Wt::WText(Wt::WString::fromUTF8(row[1]), service_table->elementAt(row_number, 2));
			new Wt::WText(Wt::WString::fromUTF8(row[3]), service_table->elementAt(row_number, 3));
			new Wt::WText(Wt::WString::fromUTF8(row[4]), service_table->elementAt(row_number, 4));
			new Wt::WText(Wt::WString::fromUTF8(row[5]), service_table->elementAt(row_number, 5));
		}
		service_table->refresh();
		mysql_free_result(res);
		mysql_close(conn);
	}

	delete dialog;
    }));

    dialog->show();
}







// show dialog window where you manage services (create)
extern void WtAccounts::manage_services_dialog()
{
    Wt::WDialog *dialog = new Wt::WDialog(Wt::WString::fromUTF8("Услуги"));

    dialog->resize(1050, 650);



    // srevice top buttons
    Wt::WPushButton *save_exit_button = new Wt::WPushButton(Wt::WString::fromUTF8("Сохранить и закрыть"), dialog->contents());
    save_exit_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    Wt::WPushButton *add_service_button = new Wt::WPushButton(Wt::WString::fromUTF8("Добавить"), dialog->contents());
    add_service_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));

    //Wt::WPushButton *change_ip_button = new Wt::WPushButton(Wt::WString::fromUTF8("Изменить"), dialog->contents());
    //change_ip_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-1 col-lg-1"));

    //Wt::WPushButton *delete_service_button = new Wt::WPushButton(Wt::WString::fromUTF8("Удалить"), dialog->contents());
    //delete_service_button->setStyleClass(Wt::WString::fromUTF8("btn btn-default with-label col-md-3 col-lg-3"));


	add_service_button->clicked().connect(boost::bind(&WtAccounts::create_new_service_dialog, this));
	//delete_service_button->clicked().connect(boost::bind(&WtAccounts::delete_service_dialog, this, operation_name));



	mysql_init(&mysql);
	conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
	if(conn==NULL)
	{
	    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
	}
	mysql_query(&mysql,"SET NAMES 'UTF8'");




	// table with data about ip address
	service_table = new Wt::WTable(dialog->contents());
	service_table->setHeaderCount(1);
	service_table->setWidth(Wt::WLength("100%"));
	//service_table->setHeight(Wt::WLength("100px"));
	service_table->resize(1000, 100);
	//service_table


	service_table->setHeaderCount(1);

	//service_table->resize();
	service_table->elementAt(0, 0)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Название")));
	service_table->elementAt(0, 1)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Период начисления")));
	service_table->elementAt(0, 2)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Код")));
	service_table->elementAt(0, 3)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Тариф")));
	service_table->elementAt(0, 4)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Фикс. сумма")));
	service_table->elementAt(0, 5)->addWidget(new Wt::WText(Wt::WString::fromUTF8("Валюта")));

	int row_number = 0;



	std::string mysql_service_table_data = "SELECT service_name, code, charging_period, "
				"service_tariff, amount, currency FROM subscriber_services";

	query_state=mysql_query(conn, mysql_service_table_data.c_str());
	if(query_state!=0)
	{
	   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
	}
	res=mysql_store_result(conn);
	while((row=mysql_fetch_row(res))!=NULL)
	{
		row_number++;
		new Wt::WText(Wt::WString::fromUTF8(row[0]), service_table->elementAt(row_number, 0));
		new Wt::WText(Wt::WString::fromUTF8(row[2]), service_table->elementAt(row_number, 1));
		new Wt::WText(Wt::WString::fromUTF8(row[1]), service_table->elementAt(row_number, 2));
		new Wt::WText(Wt::WString::fromUTF8(row[3]), service_table->elementAt(row_number, 3));
		new Wt::WText(Wt::WString::fromUTF8(row[4]), service_table->elementAt(row_number, 4));
		new Wt::WText(Wt::WString::fromUTF8(row[5]), service_table->elementAt(row_number, 5));
	}
	row_number = 0;
	service_table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped top_mar top_pad");


	mysql_free_result(res);
	mysql_close(conn);;



    dialog->contents()->addStyleClass("form-group");



    //Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
    //ok->setDefault(true);


    Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
    dialog->rejectWhenEscapePressed();



    // Accept the dialog
    save_exit_button->clicked().connect(std::bind([=] () {
	    dialog->accept();
    }));


    // Reject the dialog
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    // Process the dialog result.
    dialog->finished().connect(std::bind([=] () {
	if (dialog->result() == Wt::WDialog::Accepted)
	{
	}

	delete dialog;
    }));

    dialog->show();
}









// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






/*
extern void WtAccounts::subscriber_fullName_changed() {
	//treeTable->tree()->itemSelectionChanged()
    Wt::WMessageBox *messageBox;
	std::string changedSubscriberName = "";
	Wt::WTreeNode *selected_node; // operator* returns contents of an interator
	std::set<Wt::WTreeNode* > highlightedRows = ui->user_treeTable->tree()->selectedNodes();
	if (!highlightedRows.empty())
	{
		for (std::set<Wt::WTreeNode* >::iterator i = highlightedRows.begin(); i != highlightedRows.end(); ++i)
		{
		  selected_node = *i;
		}


		changedSubscriberName = selected_node->label()->text().toUTF8();

		highlightedRows.clear();









		mysql_init(&mysql);
		conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
		if(conn==NULL)
		{
		    std::cout<<mysql_error(&mysql)<<std::endl<<std::endl;
		}
		mysql_query(&mysql,"SET NAMES 'UTF8'");


	// add node form mysql table
		std::string mysql_get_subscriber_info = "SELECT subscriber_id, full_name, account_number, tariff_plan, date_since, date_by FROM subscriber WHERE subscriber.full_name = '"+changedSubscriberName+"'";
		query_state=mysql_query(conn, mysql_get_subscriber_info.c_str());
		if(query_state!=0)
		{
		   std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		res=mysql_store_result(conn);
		std::cout<<"MySQL Values in the amaDB Table."<<std::endl<<std::endl;
		row=mysql_fetch_row(res);

		std::string subscriber_id_view_mode = boost::lexical_cast<std::string>(row[0]);
		ui->user_info_groupbox->setTitle(Wt::WString::fromUTF8("Абонент ") + Wt::WString::fromUTF8(row[1]));
		ui->personal_account_edit->setText(Wt::WString::fromUTF8(row[2]));
		ui->user_name_edit->setText(Wt::WString::fromUTF8(row[1]));
		ui->tariff_edit->setText(Wt::WString::fromUTF8(row[3]));

		if (boost::lexical_cast<std::string>(row[4]) == "0000-00-00")
		{
			ui->include_date_edit->setText("");
		} else
		{
			ui->include_date_edit->setText(Wt::WString::fromUTF8(row[4]));
		}

		if (boost::lexical_cast<std::string>(row[5]) == "0000-00-00")
		{
			ui->disable_date_edit->setText("");
		} else
		{
			ui->disable_date_edit->setText(Wt::WString::fromUTF8(row[5]));
		}

		//mysql_free_result(res);



		std::string mysql_get_ip_address_and_phone_numbers = "SELECT ip_address, number FROM ip_addresses, phone_numbers "
				"WHERE ip_addresses.subscriber_id = '"+subscriber_id_view_mode+"' "
				"AND ip_addresses.subscriber_id = phone_numbers.subscriber_id";


		query_state=mysql_query(conn, mysql_get_ip_address_and_phone_numbers.c_str());

		std::string ip_and_numbers = "";
		if(query_state!=0)
		{
			std::cout<<mysql_error(conn)<<std::endl<<std::endl;
		}
		add_res=mysql_store_result(conn);
		row=mysql_fetch_row(add_res);
		if (row != NULL)
		{
			if (boost::lexical_cast<std::string>(row[0])!= "") ip_and_numbers.append(boost::lexical_cast<std::string>(row[0]));
			if (boost::lexical_cast<std::string>(row[0]) != "" && boost::lexical_cast<std::string>(row[1]) != "")
			{
				ip_and_numbers.append(", ");
				ip_and_numbers.append(boost::lexical_cast<std::string>(row[1]));
			}
		}
		ui->ip_number_edit->setText(Wt::WString::fromUTF8(ip_and_numbers));

		mysql_free_result(res);
		mysql_free_result(add_res);

		mysql_close(conn);











/*
		messageBox = new Wt::WMessageBox(Wt::WString::fromUTF8("Adilbek cool: "), Wt::WString::fromUTF8(changedSubscriberName), Wt::Information, Wt::Yes | Wt::No);
	    messageBox->buttonClicked().connect(std::bind([=] () {
		delete messageBox;
	    }));

	    messageBox->show();
*/

//	}
	//+ Wt::WString::fromUTF8(changedSubscriberName)




//}
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^







// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

