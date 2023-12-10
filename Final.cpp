#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <stdlib.h>
using namespace std;
const char separatore = '"';
const char separatore_csv = ';';
string nome_pagina_hmi = "Hmi_Pagina_";
string nome_pagina_hmi_xml = " ";
string prev_page = "Hmi_Pagina";
string next_page = "";
string matrice = "";
string nome_file_excel = "Test.csv";
int numero_riga_excel = 0;
int numero_colonna_excel = 0;
const int screen_width = 1366;
const int screen_height = 768;
const int Y_t = 25;
const int X_t = 300;
const int Z = 25;
const int d_w_new = 10;
const int d_h_new = 6;
const int X_r0 = 5;
const int Y_r0 = 5;
const int Radius = Z/2;
int page = 0;
int index = 0;
int Space = 10;
int n_pagine= 0;
int width_buttons = 55 ;
int height_buttons = 30;
int Prev_T = 730;
int Prev_L = 1180;
int Next_T = 730;
int Next_L = 1307;
int Home_L = 1244;
int Home_T = 730;
int ID = 4;
int cont_cerchio = 1;
int cont_button = 1;
int cont_testo = 1;
int cont_rect = 1;
int indice = 0;
int n_elements = 0;
int n_cassetti = 0;
int numero_pag = 1;

struct cassetto{
	int lunghezza;
	string *nomi;
	int Height;
	int Width;
	int Area;
	int Left;
	int Top;
	int maxlen;
	string Tag;
	string Tipo;
};
cassetto box[500];

struct pages{
	int *cols_per_rows ;
	int *top_per_rows ;
	int *left_per_cols;
	int row_tot;
	int col_tot;
	cassetto *riquadro;
	int n_riquadri;
};
pages *Pagine;


void Start()
{
	cout<<"\nInserire nome del file csv da importare con estensione (es. test.csv) : ";
	cin>>nome_file_excel;
	
	cout<<"\nInserisci il nome delle pagine hmi da creare, esso deve terminare con '_' (es. prova_) : ";
	cin>>nome_pagina_hmi;
	matrice = nome_pagina_hmi;
	
	cout<<"\nInserire numero colonna del file csv corrispondente al tipo di dati (DI,DQ,AI,AQ) : \n";
	cout<<"Numero colonna TIPO_DATI : ";
	cin>>numero_colonna_excel;
	
}

void Conta_elementi()
{
	fstream fin;
	int cont = 0;
	cout<<"Il file aperto è : "<<nome_file_excel.c_str();
	fin.open(nome_file_excel.c_str(),ios::in);
	if(fin.bad())
		cout<<"Errore, impossibile aprire/leggere il file. Controllare se il file e' gia aperto";
	while(!fin.eof())
	{
		int limite = 60;
		string temp;
		char tempor ;
		for(int i = 0;i<limite;i++)
		{
		
		fin>>tempor;
		if(tempor == separatore_csv)
		cont++;
		
		if(cont == numero_colonna_excel)
		{	
			string copia;
			getline(fin,copia,';');
			
			if(!copia.empty() and copia.size() != 0)
			n_elements++;
			
			i += limite;
			cont = 0;
		}
		}
		getline(fin,temp,'\n');
	}
}

void Estrai_colonna(int n_colonna,string riempi[])
{
	fstream fin;
	int cont = 0;
	int index = 0;
	bool first = true;
	fin.open(nome_file_excel.c_str(),ios::in);
	
	if(fin.bad())
		cout<<"Errore, impossibile aprire/leggere il file. Controllare se il file e' gia aperto";
		
	while(!fin.eof())
	{
		int limite = 60;
		string temp;
		int temp_index = 0;
		char tempor ;
		for(int i = 0;i<limite;i++)
		{
		
		fin>>tempor;
		if(tempor == separatore_csv)
		cont++;
		
		if(cont == n_colonna)
		{	
			string copia;
			getline(fin,copia,';');
			
			if(!copia.empty() and copia.size() != 0)
			riempi[index++]= copia;
			cont = 0;
			i += limite;
			//cout<<"\n Copia : "<<copia;
		}

		}
		getline(fin,temp,'\n');
	}
	//cout<<"\n"<<index;
}

void Riempi_cassetti(string tag[],string nomi_completi[],string tipo_dati[])
{
	int ind = 0;
	int index = 0;
	
	for (int i = 0;i<n_elements;i++)
	{
		// se due tag di fila sono uguali
	//	cout<<endl<<"Valore di i+1 : "<<i+1<<" next tag : "<<tag[i+1];
	if(i <=n_elements-2)
	{
			if(tag[i]==tag[i+1])
			box[index].lunghezza++;
			else // se due tag di fila non sono uguali
			{
				if(box[index].lunghezza == 0) // se due tag di fila non sono uguali e il tag è unico
				{
						box[index].nomi = new string[1];
						box[index].nomi[0] = nomi_completi[i];
						box[index].lunghezza++;
						ind = 1;
						//cout<<endl<<" Index :"<<index<< " "<<box[index].nomi[0]<< " Tag : "<<tag[i]<<" i : "<<i;
				}
				else // se due tag di fila non sono uguali e il tag non è unico
				{
					
				box[index].nomi = new string[1+box[index].lunghezza];
				cout<<"\n Lunghezza sequenza : "<<1+box[index].lunghezza;
				for (int j = i - box[index].lunghezza;j <= i;j++)
				{
					box[index].nomi[ind++] = nomi_completi[j];
					//cout<<endl<<" Index :"<<index<< " "<<box[index].nomi[ind-1]<< " Tag : "<<tag[i]<<" i : "<<i;
				}
				
				box[index].lunghezza++;
				}
			
			box[index].Tipo = tipo_dati[i];
			box[index].Tag = tag[i];
			index++;
			ind=0;	
			}
	}
	else
	{ 
				if(box[index].lunghezza == 0) // se due tag di fila non sono uguali e il tag è unico
				{
						box[index].nomi = new string[1];
						box[index].nomi[0] = nomi_completi[i];
						box[index].lunghezza++;
						ind = 1;
										}
				else // se due tag di fila non sono uguali e il tag non è unico
				{	
				box[index].nomi = new string[1+box[index].lunghezza];
				for (int j = i - box[index].lunghezza;j <= i;j++)
					box[index].nomi[ind++] = nomi_completi[j];
				box[index].lunghezza++;
				}
			
			box[index].Tipo = tipo_dati[i];
			box[index].Tag = tag[i];
			index++;
			ind=0;	
	}
}
	n_cassetti = index;
		
}

void Inizio_Gruppo()
{
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	fin<<
"          <Hmi.Screen.Group ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"            <AttributeList>\n"
"              <ObjectName>Group"<<ID++<<"</ObjectName>\n"
"              <TabIndex>-1</TabIndex>\n"
"            </AttributeList>\n"
"            <ObjectList>\n";

}

void Definizione_Pagina(int Screen_ID,string Page_name,int Page_number)
{
	ofstream fin;
	nome_pagina_hmi = matrice + to_string(numero_pag); // pagina corrente
	nome_pagina_hmi_xml = nome_pagina_hmi+".xml";
	
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	if(fin.bad())
	cout<<"\nErrore";
	fin<<
	"<?xml version="<<separatore<<"1.0"<<separatore<<" encoding="<<separatore<<"utf-8"<<separatore<<"?>\n"
	"<Document>\n"
	"  <Engineering version="<<separatore<<"V16"<<separatore<<" />\n"
	"  <DocumentInfo>\n"
	"    <Created>2021-06-29T09:40:51.540039Z</Created>\n"
	"    <ExportSetting>WithDefaults, WithReadOnly</ExportSetting>\n"
	"    <InstalledProducts>\n"
	"      <Product>\n"
	"        <DisplayName>Totally Integrated Automation Portal</DisplayName>\n"
	"        <DisplayVersion>V16 Update 3</DisplayVersion>\n"
	"      </Product>\n"
	"      <OptionPackage>\n"
	"        <DisplayName>TIA Portal Version Control Interface</DisplayName>\n"
	"        <DisplayVersion>V16 Update 2</DisplayVersion>\n"
	"      </OptionPackage>\n"
	"      <OptionPackage>\n"
	"        <DisplayName>TIA Portal Openness</DisplayName>\n"
	"        <DisplayVersion>V16 Update 3</DisplayVersion>\n"
	"      </OptionPackage>\n"
	"      <Product>\n"
	"        <DisplayName>SINAMICS Startdrive Advanced</DisplayName>\n"
	"        <DisplayVersion>V16</DisplayVersion>\n"
	"      </Product>\n"
	"      <OptionPackage>\n"
	"        <DisplayName>SINAMICS Startdrive G130, G150, S120, S150, SINAMICS MV, S210</DisplayName>\n"
	"        <DisplayVersion>V16</DisplayVersion>\n"
	"      </OptionPackage>\n"
	"      <OptionPackage>\n"
	"        <DisplayName>SINAMICS Startdrive G110M, G120, G120C, G120D, G120P</DisplayName>\n"
	"        <DisplayVersion>V16</DisplayVersion>\n"
	"      </OptionPackage>\n"
	"      <Product>\n"
	"        <DisplayName>STEP 7 Professional</DisplayName>\n"
	"        <DisplayVersion>V16 Update 3</DisplayVersion>\n"
	"      </Product>\n"
	"      <OptionPackage>\n"
	"        <DisplayName>SIMATIC Energy Suite</DisplayName>\n"
	"        <DisplayVersion>V16</DisplayVersion>\n"
	"      </OptionPackage>\n"
	"      <OptionPackage>\n"
	"        <DisplayName>STEP 7 Safety</DisplayName>\n"
	"        <DisplayVersion>V16</DisplayVersion>\n"
	"      </OptionPackage>\n"
	"      <Product>\n"
	"        <DisplayName>WinCC Advanced</DisplayName>\n"
	"        <DisplayVersion>V16 Update 3</DisplayVersion>\n"
	"      </Product>\n"
	"    </InstalledProducts>\n"
	"  </DocumentInfo>\n"
	"  <Hmi.Screen.Screen ID="<<separatore<<Screen_ID<<separatore<<">\n"
	"    <AttributeList>\n"
	"      <ActiveLayer>0</ActiveLayer>\n"
	"      <BackColor>182, 182, 182</BackColor>\n"
	"      <GridColor>0, 0, 0</GridColor>\n"
	"      <Height>768</Height>\n"
	"      <Name>"<<nome_pagina_hmi<<"</Name>\n"
	"      <Number>"<<Page_number<<"</Number>\n"
	"      <Visible>true</Visible>\n"
	"      <Width>1366</Width>\n"
	"    </AttributeList>\n"
	"    <ObjectList>\n"
	"      <MultilingualText ID="<<separatore<<1<<separatore<<" CompositionName="<<separatore<<"HelpText"<<separatore<<">\n"
	"        <ObjectList>\n"
	"          <MultilingualTextItem ID="<<separatore<<2<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
	"            <AttributeList>\n"
	"              <Culture>it-IT</Culture>\n"
	"              <Text />\n"
	"            </AttributeList>\n"
	"          </MultilingualTextItem>\n"
	"        </ObjectList>\n"
	"      </MultilingualText>\n"
	"      <Hmi.Screen.ScreenLayer ID="<<separatore<<3<<separatore<<" CompositionName="<<separatore<<"Layers"<<separatore<<">\n"
	"        <AttributeList>\n"
	"          <Index>0</Index>\n"
	"          <Name />\n"
	"          <VisibleES>true</VisibleES>\n"
	"        </AttributeList>\n"
	"        <ObjectList>\n";
	
	if(numero_pag!=1)
	prev_page =  matrice + to_string(numero_pag-1); 
	else
	prev_page = matrice + to_string(n_pagine+1);
	
	if(numero_pag!=n_pagine)
	next_page = matrice + to_string(numero_pag+1);
	else
	next_page = matrice + to_string(1);
	
	numero_pag++;
	}

void Rettangolo(int Height,int Width,int Top, int Left ,bool choice_color)
{
	
	string color = "0, 180, 0";
	if(choice_color)
	color = " 217, 217, 217";
	string Rect_name = "Rettangolo_";
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
 
fin<<				
"              <Hmi.Screen.Rectangle ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"                <AttributeList>\n"
"                  <BackColor>"<<color<<"</BackColor>\n"
"                  <BackFillStyle>Solid</BackFillStyle>\n"
"                  <BorderColor>24, 28, 49</BorderColor>\n"
"                  <BorderWidth>1</BorderWidth>\n"
"                  <EdgeStyle>Solid</EdgeStyle>\n"
"                  <Flashing>None</Flashing>\n"
"                  <Height>"<<Height<<"</Height>\n"
"                  <Left>"<<Left<<"</Left>\n"
"                  <ObjectName>"<<Rect_name<<cont_rect++<<"</ObjectName>\n"
"                  <RoundCornerHeight>"<<10<<"</RoundCornerHeight>\n"
"                  <RoundCornerWidth>"<<7<<"</RoundCornerWidth>\n"
"                  <TabIndex>-1</TabIndex>\n"
"                  <Top>"<<Top<<"</Top>\n"
"                  <UseDesignColorSchema>false</UseDesignColorSchema>\n"
"                  <Width>"<<Width<<"</Width>\n"
"                </AttributeList>\n"
"              </Hmi.Screen.Rectangle>\n";

}

void Pulsante(int Height,int Width,int Top, int Left, string PLC_tag,bool Set)
{
	string bit_operation = "ResetBit";
	string Color = "255, 0, 0";
	string testo = "Reset";
	if (Set)
	{
	bit_operation = "SetBit";
	Color = "0, 255, 0";	
	testo = "Set";
	}
	 
	
	string Button_name = "Pulsante_";
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
 
fin<<	"     <Hmi.Screen.Button ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"            <AttributeList>\n"
"              <BackColor>99, 101, 113</BackColor>\n"
"              <BackFillStyle>GradientVertical</BackFillStyle>\n"
"              <BitNumber>0</BitNumber>\n"
"              <BorderBackColor>105, 105, 105</BorderBackColor>\n"
"              <BorderColor>71, 73, 87</BorderColor>\n"
"              <BorderWidth>2</BorderWidth>\n"
"              <CornerRadius>3</CornerRadius>\n"
"              <CornerStyle>Pointed</CornerStyle>\n"
"              <EdgeStyle>Solid</EdgeStyle>\n"
"              <Enabled>true</Enabled>\n"
"              <FirstGradientColor>131, 132, 142</FirstGradientColor>\n"
"              <FirstGradientOffset>15</FirstGradientOffset>\n"
"              <FitToLargest>false</FitToLargest>\n"
"              <Flashing>None</Flashing>\n"
"              <FocusColor>148, 182, 231</FocusColor>\n"
"              <FocusWidth>2</FocusWidth>\n"
"              <ForeColor>255, 255, 255</ForeColor>\n"
"              <Height>"<<Height<<"</Height>\n"
"              <HorizontalAlignment>Center</HorizontalAlignment>\n"
"              <HorizontalPictureAlignment>Center</HorizontalPictureAlignment>\n"
"              <HotKey>0</HotKey>\n"
"              <Left>"<<Left<<"</Left>\n"
"              <MiddleGradientColor>"<<Color<<"</MiddleGradientColor>\n"
"              <Mode>Text</Mode>\n"
"              <ObjectName>"<<Button_name<<cont_button++<<"</ObjectName>\n"
"              <PictureAreaBottomMargin>0</PictureAreaBottomMargin>\n"
"              <PictureAreaLeftMargin>0</PictureAreaLeftMargin>\n"
"              <PictureAreaRightMargin>0</PictureAreaRightMargin>\n"
"              <PictureAreaTopMargin>0</PictureAreaTopMargin>\n"
"              <PictureAutoSizing>StretchPicture</PictureAutoSizing>\n"
"              <SecondGradientColor>88, 90, 103</SecondGradientColor>\n"
"              <SecondGradientOffset>15</SecondGradientOffset>\n"
"              <TabIndex>1</TabIndex>\n"
"              <TextAreaBottomMargin>0</TextAreaBottomMargin>\n"
"              <TextAreaLeftMargin>0</TextAreaLeftMargin>\n"
"              <TextAreaRightMargin>0</TextAreaRightMargin>\n"
"              <TextAreaTopMargin>0</TextAreaTopMargin>\n"
"              <TextOrientation>Horizontal</TextOrientation>\n"
"              <Top>"<<Top<<"</Top>\n"
"              <UseDesignColorSchema>false</UseDesignColorSchema>\n"
"              <UseFirstGradient>false</UseFirstGradient>\n"
"              <UseSecondGradient>false</UseSecondGradient>\n"
"              <UseTwoHandOperation>false</UseTwoHandOperation>\n"
"              <VerticalAlignment>Middle</VerticalAlignment>\n"
"              <VerticalPictureAlignment>Middle</VerticalPictureAlignment>\n"
"              <Width>"<<Width<<"</Width>\n"
"            </AttributeList>\n"
"            <ObjectList>\n"
"              <Hmi.Event.Event ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Events"<<separatore<<">\n"
"                <AttributeList>\n"
"                  <Name>Press</Name>\n"
"                </AttributeList>\n"
"                <ObjectList>\n"
"                  <Hmi.Event.FunctionListEventHandler ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"EventHandler"<<separatore<<">\n"
"                    <ObjectList>\n"
"                      <Hmi.Event.FunctionListEntry ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"FunctionListEntries"<<separatore<<">\n"
"                        <AttributeList>\n"
"                          <Name>"<<bit_operation<<"</Name>\n"
"                          <Type>SystemFunction</Type>\n"
"                        </AttributeList>\n"
"                        <ObjectList>\n"
"                          <Hmi.Event.FunctionListEntryParameter ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Parameters"<<separatore<<">\n"
"                            <AttributeList>\n"
"                              <Name>Tag</Name>\n"
"                            </AttributeList>\n"
"                            <LinkList>\n"
"                              <Value TargetID="<<separatore<<"@OpenLink"<<separatore<<">\n"
"                                <Name>"<<separatore<<PLC_tag<<separatore<<"</Name>\n"
"                              </Value>\n"
"                            </LinkList>\n"
"                          </Hmi.Event.FunctionListEntryParameter>\n"
"                        </ObjectList>\n"
"                      </Hmi.Event.FunctionListEntry>\n"
"                    </ObjectList>\n"
"                  </Hmi.Event.FunctionListEventHandler>\n"
"                </ObjectList>\n"
"              </Hmi.Event.Event>\n"
"              <Hmi.Globalization.MultiLingualFont ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Font"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <Hmi.Globalization.FontItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <FontFamily>Tahoma</FontFamily>\n"
"                      <FontSize>17</FontSize>\n"
"                      <FontStyle>Bold</FontStyle>\n"
"                    </AttributeList>\n"
"                  </Hmi.Globalization.FontItem>\n"
"                </ObjectList>\n"
"              </Hmi.Globalization.MultiLingualFont>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"HelpText"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text />\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"TextOff"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text><body><p>"<<testo<<"</p></body></Text>\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"TextOn"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text><body><p>Text</p></body></Text>\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"            </ObjectList>\n"
"          </Hmi.Screen.Button>\n";

}

void Pulsante_pagine (int Height,int Width,int Top, int Left, string nome_pag,int choice)
{
	// 1 next
	// 2 prev
	// 3 home
	string testo = "";
	string Color = "";
	if (choice==1)
	{
		Color = "0, 255, 0";
		testo = "Next";
	}
		
	if(choice==2)
	{
		Color = "255, 153, 0";	
		testo = "Prev";
	}
		
	if(choice==3)
	{
		Color = "51, 102, 255";	
		testo = "Home";
	}
	
	string Button_name = "Pulsante_";
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);

fin<<
"			<Hmi.Screen.Button ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"            <AttributeList>\n"
"              <BackColor>99, 101, 113</BackColor>\n"
"              <BackFillStyle>GradientVertical</BackFillStyle>\n"
"              <BitNumber>0</BitNumber>\n"
"              <BorderBackColor>105, 105, 105</BorderBackColor>\n"
"              <BorderColor>71, 73, 87</BorderColor>\n"
"              <BorderWidth>2</BorderWidth>\n"
"              <CornerRadius>3</CornerRadius>\n"
"              <CornerStyle>Pointed</CornerStyle>\n"
"              <EdgeStyle>Solid</EdgeStyle>\n"
"              <Enabled>true</Enabled>\n"
"              <FirstGradientColor>131, 132, 142</FirstGradientColor>\n"
"              <FirstGradientOffset>15</FirstGradientOffset>\n"
"              <FitToLargest>false</FitToLargest>\n"
"              <Flashing>None</Flashing>\n"
"              <FocusColor>148, 182, 231</FocusColor>\n"
"              <FocusWidth>2</FocusWidth>\n"
"              <ForeColor>255, 255, 255</ForeColor>\n"
"              <Height>"<<Height<<"</Height>\n"
"              <HorizontalAlignment>Center</HorizontalAlignment>\n"
"              <HorizontalPictureAlignment>Center</HorizontalPictureAlignment>\n"
"              <HotKey>0</HotKey>\n"
"              <Left>"<<Left<<"</Left>\n"
"              <MiddleGradientColor>"<<Color<<"</MiddleGradientColor>\n"
"              <Mode>Text</Mode>\n"
"              <ObjectName>"<<Button_name<<cont_button++<<"</ObjectName>\n"
"              <PictureAreaBottomMargin>0</PictureAreaBottomMargin>\n"
"              <PictureAreaLeftMargin>0</PictureAreaLeftMargin>\n"
"              <PictureAreaRightMargin>0</PictureAreaRightMargin>\n"
"              <PictureAreaTopMargin>0</PictureAreaTopMargin>\n"
"              <PictureAutoSizing>StretchPicture</PictureAutoSizing>\n"
"              <SecondGradientColor>88, 90, 103</SecondGradientColor>\n"
"              <SecondGradientOffset>15</SecondGradientOffset>\n"
"              <TabIndex>1</TabIndex>\n"
"              <TextAreaBottomMargin>0</TextAreaBottomMargin>\n"
"              <TextAreaLeftMargin>0</TextAreaLeftMargin>\n"
"              <TextAreaRightMargin>0</TextAreaRightMargin>\n"
"              <TextAreaTopMargin>0</TextAreaTopMargin>\n"
"              <TextOrientation>Horizontal</TextOrientation>\n"
"              <Top>"<<Top<<"</Top>\n"
"              <UseDesignColorSchema>false</UseDesignColorSchema>\n"
"              <UseFirstGradient>false</UseFirstGradient>\n"
"              <UseSecondGradient>false</UseSecondGradient>\n"
"              <UseTwoHandOperation>false</UseTwoHandOperation>\n"
"              <VerticalAlignment>Middle</VerticalAlignment>\n"
"              <VerticalPictureAlignment>Middle</VerticalPictureAlignment>\n"
"              <Width>"<<Width<<"</Width>\n"
"            </AttributeList>\n"
"            <ObjectList>\n"
"              <Hmi.Event.Event ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Events"<<separatore<<">\n"
"                <AttributeList>\n"
"                  <Name>Click</Name>\n"
"                </AttributeList>\n"
"                <ObjectList>\n"
"                  <Hmi.Event.FunctionListEventHandler ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"EventHandler"<<separatore<<">\n"
"                    <ObjectList>\n"
"                      <Hmi.Event.FunctionListEntry ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"FunctionListEntries"<<separatore<<">\n"
"                        <AttributeList>\n"
"                          <Name>ActivateScreen</Name>\n"
"                          <Type>SystemFunction</Type>\n"
"                        </AttributeList>\n"
"                        <ObjectList>\n"
"                          <Hmi.Event.FunctionListEntryParameter ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Parameters"<<separatore<<">\n"
"                            <AttributeList>\n"
"                              <Name>Screen name</Name>\n"
"                            </AttributeList>\n"
"                            <LinkList>\n"
"                              <Value TargetID="<<separatore<<"@OpenLink"<<separatore<<">\n"
"                                <Name>"<<nome_pag<<"</Name>\n"
"                              </Value>\n"
"                            </LinkList>\n"
"                          </Hmi.Event.FunctionListEntryParameter>\n"
"                          <Hmi.Event.FunctionListEntryParameter ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Parameters"<<separatore<<">\n"
"                            <AttributeList>\n"
"                              <Name>Object number</Name>\n"
"                              <Value Type="<<separatore<<"System.Int32"<<separatore<<">0</Value>\n"
"                            </AttributeList>\n"
"                          </Hmi.Event.FunctionListEntryParameter>\n"
"                        </ObjectList>\n"
"                      </Hmi.Event.FunctionListEntry>\n"
"                    </ObjectList>\n"
"                  </Hmi.Event.FunctionListEventHandler>\n"
"                </ObjectList>\n"
"              </Hmi.Event.Event>\n"
"              <Hmi.Globalization.MultiLingualFont ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Font"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <Hmi.Globalization.FontItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <FontFamily>Tahoma</FontFamily>\n"
"                      <FontSize>17</FontSize>\n"
"                      <FontStyle>Bold</FontStyle>\n"
"                    </AttributeList>\n"
"                  </Hmi.Globalization.FontItem>\n"
"                </ObjectList>\n"
"              </Hmi.Globalization.MultiLingualFont>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"HelpText"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text />\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"TextOff"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text><body><p>"<<testo<<"</p></body></Text>\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"TextOn"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text><body><p>Text</p></body></Text>\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"            </ObjectList>\n"
"          </Hmi.Screen.Button>\n";

}

void Cerchio(int Height,int Top, string PLC_tag, int Left,int Width)
{
	string Cerchio_name = "Cerchio_";
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	fin<<
	"              <Hmi.Screen.Circle ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"                  <AttributeList>\n"
"                  <BackColor>217, 217, 217</BackColor>\n"
"                  <BackFillStyle>Solid</BackFillStyle>\n"
"                  <BorderColor>24, 28, 49</BorderColor>\n"
"                  <BorderWidth>1</BorderWidth>\n"
"                  <EdgeStyle>Solid</EdgeStyle>\n"
"                  <Flashing>None</Flashing>\n"
"                  <Height>"<<Height<<"</Height>\n"
"                  <Left>"<<Left<<"</Left>\n"
"                  <ObjectName>"<<Cerchio_name<<cont_cerchio++<<"</ObjectName>\n"
"                  <Radius>"<<Radius<<"</Radius>\n"
"                  <TabIndex>-1</TabIndex>\n"
"                  <Top>"<<Top<<"</Top>\n"
"                  <UseDesignColorSchema>false</UseDesignColorSchema>\n"
"                  <Width>"<<Width<<"</Width>\n"
"                </AttributeList>\n"
"                <ObjectList>\n"
"                  <Hmi.Dynamic.RangeAppearanceAnimation ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Animations"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Name>RangeAppearanceAnimation</Name>\n"
"                    </AttributeList>\n"
"                    <ObjectList>\n"
"                      <Hmi.Dynamic.TagElementTrigger ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"RangeTag"<<separatore<<">\n"
"                        <LinkList>\n"
"                          <Tag TargetID="<<separatore<<"@OpenLink"<<separatore<<">\n"
"                            <Name>"<<separatore<<PLC_tag<<separatore<<"</Name>\n"
"                          </Tag>\n"
"                        </LinkList>\n"
"                      </Hmi.Dynamic.TagElementTrigger>\n"
"                      <Hmi.Dynamic.Range ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"RangeValues"<<separatore<<">\n"
"                        <AttributeList>\n"
"                          <BackColor>255, 0, 0</BackColor>\n"
"                          <FlashingType>No</FlashingType>\n"
"                          <ForeColor>24, 28, 49</ForeColor>\n"
"                          <LowerLimit>0</LowerLimit>\n"
"                          <UpperLimit>0</UpperLimit>\n"
"                        </AttributeList>\n"
"                      </Hmi.Dynamic.Range>\n"
"                      <Hmi.Dynamic.Range ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"RangeValues"<<separatore<<">\n"
"                        <AttributeList>\n"
"                          <BackColor>0, 255, 0</BackColor>\n"
"                          <FlashingType>No</FlashingType>\n"
"                          <ForeColor>24, 28, 49</ForeColor>\n"
"                          <LowerLimit>1</LowerLimit>\n"
"                          <UpperLimit>1</UpperLimit>\n"
"                        </AttributeList>\n"
"                      </Hmi.Dynamic.Range>\n"
"                    </ObjectList>\n"
"                  </Hmi.Dynamic.RangeAppearanceAnimation>\n"
"                </ObjectList>\n"
"              </Hmi.Screen.Circle>\n";	
	
	}

void Testo(string Body_text, int Height,int Width,int Top, int Left)
{
	string Text_name = "Testo_";
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	fin<<
	
	"              <Hmi.Screen.TextField ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"                 <AttributeList>\n"
"                  <BackColor>255, 255, 255</BackColor>\n"
"                  <BackFillStyle>Transparent</BackFillStyle>\n"
"                  <BorderBackColor>101, 103, 115</BorderBackColor>\n"
"                  <BorderColor>71, 73, 87</BorderColor>\n"
"                  <BorderWidth>0</BorderWidth>\n"
"                  <BottomMargin>2</BottomMargin>\n"
"                  <CornerRadius>3</CornerRadius>\n"
"                  <EdgeStyle>Double</EdgeStyle>\n"
"                  <FitToLargest>true</FitToLargest>\n"
"                  <Flashing>None</Flashing>\n"
"                  <ForeColor>49, 52, 74</ForeColor>\n"
"                  <Height>"<<Height<<"</Height>\n"
"                  <HorizontalAlignment>Left</HorizontalAlignment>\n"
"                  <Left>"<<Left<<"</Left>\n"
"                  <LeftMargin>3</LeftMargin>\n"
"                  <ObjectName>"<<Text_name<<cont_testo++<<"</ObjectName>\n"
"                  <RightMargin>2</RightMargin>\n"
"                  <TabIndex>-1</TabIndex>\n"
"                  <TextOrientation>Horizontal</TextOrientation>\n"
"                  <Top>"<<Top<<"</Top>\n"
"                  <TopMargin>2</TopMargin>\n"
"                  <UseDesignColorSchema>false</UseDesignColorSchema>\n"
"                  <VerticalAlignment>Middle</VerticalAlignment>\n"
"                  <Width>"<<Width<<"</Width>\n"
"                </AttributeList>\n"
"                <ObjectList>\n"
"                  <Hmi.Globalization.MultiLingualFont ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Font"<<separatore<<">\n"
"                    <ObjectList>\n"
"                      <Hmi.Globalization.FontItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                        <AttributeList>\n"
"                          <Culture>it-IT</Culture>\n"
"                          <FontFamily>Tahoma</FontFamily>\n"
"                          <FontSize>17</FontSize>\n"
"                          <FontStyle>Bold</FontStyle>\n"
"                        </AttributeList>\n"
"                      </Hmi.Globalization.FontItem>\n"
"                    </ObjectList>\n"
"                  </Hmi.Globalization.MultiLingualFont>\n"
"                  <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Text"<<separatore<<">\n"
"                    <ObjectList>\n"
"                      <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                        <AttributeList>\n"
"                          <Culture>it-IT</Culture>\n"
"                          <Text><body><p>"<<Body_text<<"</p></body></Text>\n"
"                        </AttributeList>\n"
"                      </MultilingualTextItem>\n"
"                    </ObjectList>\n"
"                  </MultilingualText>\n"
"                </ObjectList>\n"
"              </Hmi.Screen.TextField>\n";
}

void IO_field(int Height,int Top, string PLC_tag, int Left,int Width,bool Input,bool signum)
{
	string Mode = "Input";
	if(!Input)
	string Mode = "Output";
	
	
	string signum_w = "99999";
	if (signum) 
	signum_w = "s9999";
	string Text_name = "Testo_";
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	fin<<
	"<Hmi.Screen.IOField ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"ScreenItems"<<separatore<<">\n"
"            <AttributeList>\n"
"              <AboveUpperLimitColor>237, 88, 97</AboveUpperLimitColor>\n"
"              <BackColor>255, 255, 255</BackColor>\n"
"              <BackFillStyle>Solid</BackFillStyle>\n"
"              <BelowLowerLimitColor>241, 161, 44</BelowLowerLimitColor>\n"
"              <BorderBackColor>101, 103, 115</BorderBackColor>\n"
"              <BorderColor>71, 73, 87</BorderColor>\n"
"              <BorderWidth>4</BorderWidth>\n"
"              <BottomMargin>2</BottomMargin>\n"
"              <CornerRadius>3</CornerRadius>\n"
"              <DataFormat>Decimal</DataFormat>\n"
"              <EdgeStyle>Double</EdgeStyle>\n"
"              <Enabled>true</Enabled>\n"
"              <FieldLength>5</FieldLength>\n"
"              <FitToLargest>false</FitToLargest>\n"
"              <Flashing>None</Flashing>\n"
"              <ForeColor>49, 52, 74</ForeColor>\n"
"              <FormatPattern>"<<signum_w<<"</FormatPattern>\n"
"              <Height>"<<Height<<"</Height>\n"
"              <HiddenInput>false</HiddenInput>\n"
"              <HorizontalAlignment>Left</HorizontalAlignment>\n"
"              <Left>"<<Left<<"</Left>\n"
"              <LeftMargin>3</LeftMargin>\n"
"              <Mode>"<<Mode<<"</Mode>\n"
"              <ObjectName>"<<Text_name<<cont_testo++<<"</ObjectName>\n"
"              <RightMargin>2</RightMargin>\n"
"              <ShiftDecimalPoint>0</ShiftDecimalPoint>\n"
"              <ShowLeadingZeros>false</ShowLeadingZeros>\n"
"              <TabIndex>-1</TabIndex>\n"
"              <TextOrientation>Horizontal</TextOrientation>\n"
"              <Top>"<<Top<<"</Top>\n"
"              <TopMargin>2</TopMargin>\n"
"              <Unit />\n"
"              <UseDesignColorSchema>false</UseDesignColorSchema>\n"
"              <UseTwoHandOperation>false</UseTwoHandOperation>\n"
"              <VerticalAlignment>Middle</VerticalAlignment>\n"
"              <Width>"<<Width<<"</Width>\n"
"            </AttributeList>\n"
"            <ObjectList>\n"
"              <Hmi.Globalization.MultiLingualFont ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Font"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <Hmi.Globalization.FontItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <FontFamily>Tahoma</FontFamily>\n"
"                      <FontSize>17</FontSize>\n"
"                      <FontStyle>Bold</FontStyle>\n"
"                    </AttributeList>\n"
"                  </Hmi.Globalization.FontItem>\n"
"                </ObjectList>\n"
"              </Hmi.Globalization.MultiLingualFont>\n"
"              <MultilingualText ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"HelpText"<<separatore<<">\n"
"                <ObjectList>\n"
"                  <MultilingualTextItem ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Items"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Culture>it-IT</Culture>\n"
"                      <Text />\n"
"                    </AttributeList>\n"
"                  </MultilingualTextItem>\n"
"                </ObjectList>\n"
"              </MultilingualText>\n"
"              <Hmi.Screen.Property ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Properties"<<separatore<<">\n"
"			<AttributeList>\n"
"                  <Name>ProcessValue</Name>\n"
"                </AttributeList>\n"
"                <ObjectList>\n"
"                  <Hmi.Dynamic.TagConnectionDynamic ID="<<separatore<<ID++<<separatore<<" CompositionName="<<separatore<<"Dynamic"<<separatore<<">\n"
"                    <AttributeList>\n"
"                      <Indirect>false</Indirect>\n"
"                    </AttributeList>\n"
"                    <LinkList>\n"
"                      <Tag TargetID="<<separatore<<"@OpenLink"<<separatore<<">\n"
"                        <Name>"<<PLC_tag<<"</Name>\n"
"                      </Tag>\n"
"                    </LinkList>\n"
"                  </Hmi.Dynamic.TagConnectionDynamic>\n"
"                </ObjectList>\n"
"              </Hmi.Screen.Property>\n"
"            </ObjectList>\n"
"          </Hmi.Screen.IOField>\n";
}


void Fine_Gruppo()
{
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	fin<<
"            </ObjectList>"
"          </Hmi.Screen.Group>";

}

void Chiusura()
{
	ofstream fin;
	fin.open(nome_pagina_hmi_xml.c_str(),ios::out|ios::app);	
	fin<<
	"        </ObjectList>\n"
					"      </Hmi.Screen.ScreenLayer>\n"
					"    </ObjectList>\n"
					"  </Hmi.Screen.Screen>\n"
					"</Document>\n";
}

void Assegna_Pagine()
{
	int i = 0;
	int max_height = 0;
	int x0_h = 65;
	int x0_w = 30;
	int temp = x0_w;
	int row = 0;
	Pagine = new pages[200];
	for(int i = 0;i<200;i++)
	Pagine[i].riquadro = new cassetto [50];
	int n_riquadro = 0;
	bool height_exceed = false;
	
	while(i<n_cassetti)
	{
		while(box[i].Width+temp<=screen_width-10 && !height_exceed)
		{
			if(max_height<box[i].Height) // Altezza massima per riga
			max_height = box[i].Height;

			if(x0_h+box[i].Height>screen_height-3 or (box[i].Width+temp>Prev_L and x0_h+box[i].Height> Next_T)) // se nell'ultima colonna un elemento qualsiasi non entra
			{                                                                                                 // se nell'ultima riga  non c'è spazio per i pulsanti
			 height_exceed = true;
			 x0_h = screen_height;
			}	 
			 else // nuovo elemento in una qualsiasi riga
			 {
			Pagine[n_pagine].riquadro[n_riquadro].Top = x0_h;			
			Pagine[n_pagine].riquadro[n_riquadro].Left = temp;
			Pagine[n_pagine].riquadro[n_riquadro].Height = box[i].Height;
			Pagine[n_pagine].riquadro[n_riquadro].lunghezza = box[i].lunghezza;
			Pagine[n_pagine].riquadro[n_riquadro].Width = box[i].Width;
			Pagine[n_pagine].riquadro[n_riquadro].nomi = new string[box[i].lunghezza];
			Pagine[n_pagine].riquadro[n_riquadro].Tag = box[i].Tag;
			Pagine[n_pagine].riquadro[n_riquadro].maxlen = box[i].maxlen*12;
			Pagine[n_pagine].riquadro[n_riquadro].Tipo = box[i].Tipo;

			for(int k = 0;k<box[i].lunghezza;k++) // copia i nomi per ogni riquadro
			Pagine[n_pagine].riquadro[n_riquadro].nomi[k] = box[i].nomi[k];
			
			
			for(int k = 0;k<box[i].lunghezza;k++)
			cout<<"\nPagine : "<<n_pagine<<" "<<Pagine[n_pagine].riquadro[n_riquadro].nomi[k]<<" box : "<<box[i].nomi[k];
			temp += box[i].Width + Space;
			
			if(i >= n_cassetti) // ultima pag
			{
			Pagine[n_pagine].n_riquadri = ++n_riquadro; // n_riquadri per l'ultima pagina
			break;	
			}
			
			i++; //nuovo box

			n_riquadro++;
			
			if(n_riquadro>50)
			cout<<"\nOut of riquadro memory";
		}
		//	cout<<"\nValore riga : "<<x0_w+box[i].Width+temp+Space;
		}
		if(!height_exceed)
			{
			if(x0_w+box[i].Width+temp+Space>screen_width-10)  //NUOVA RIGA
			{
				x0_h += max_height+15;
				x0_w = 30;
				temp = x0_w;
				max_height = 0;

			}
			else  // STESSA RIGA
			Pagine[n_pagine].riquadro[n_riquadro].Left = box[i].Width+temp;
			}
		
		if(x0_h>screen_height-3) // Nuova Pagina
		{
			height_exceed = false;
			Pagine[n_pagine].n_riquadri = n_riquadro;
			n_pagine++;
			n_riquadro = 0;
			x0_h = 65;
			x0_w = 30;
			temp = x0_w;
			max_height = 0;
			if(box[i].Height>screen_height)
			i++; //salta il box se non entra nello schermo
		}

	}
}

void Inizializzazione()
{
	int max = 0;
	for(int i = 0;i<n_cassetti;i++)
	{
		for(int j = 0;j<box[i].lunghezza;j++)
		{
			if(max<box[i].nomi[j].size())
			max = box[i].nomi[j].size();
		
		}
	box[i].maxlen = max;
	max = 0;
	}

	for(int i = 0;i<n_cassetti;i++)
	{
		if(box[i].Tipo == "DI")
		{
		box[i].Height = box[i].lunghezza*(1+Y_t)+4*Space;
		box[i].Width = box[i].maxlen*12+Z+2*Space;
		}
		
		if(box[i].Tipo == "DQ")
		{
		box[i].Height = box[i].lunghezza*(1+35)+3*Space;	
		box[i].Width = box[i].maxlen*12+3*Space+110;	
		}
		
		if(box[i].Tipo == "AI")
		{
		box[i].Height = box[i].lunghezza*(1+35)+3*Space;	
		box[i].Width = box[i].maxlen*12+3*Space+85;	
		}
		
		if(box[i].Tipo == "AQ")
		{
		box[i].Height = box[i].lunghezza*(1+35)+3*Space;	
		box[i].Width = box[i].maxlen*12+3*Space+85;	
		}
		
	}		
	Assegna_Pagine();
}		

void Parametriche()
{
	int page_number = 60;
	for(int i = 0;i<=n_pagine;i++)
	{
		//Crea_Pagina();
		Definizione_Pagina(0,"Prova Openness",page_number++);
		for(int j = 0;j<Pagine[i].n_riquadri;j++)
		{
			Inizio_Gruppo();
			// Rettangolo ttolo verde
			int height_title = Y_t-5;
			Rettangolo(height_title,Pagine[i].riquadro[j].Width,Pagine[i].riquadro[j].Top,Pagine[i].riquadro[j].Left,0);
			//Titolo
			int Titolo_Width = Pagine[i].riquadro[j].Tag.size()*14;
			int Titolo_Left = Pagine[i].riquadro[j].Left+10;
			int Titolo_Top = Pagine[i].riquadro[j].Top+2;
			int Titolo_Height = Y_t;
			
			Testo(Pagine[i].riquadro[j].Tag,Titolo_Height,Titolo_Width,Titolo_Top-4,Titolo_Left);
			// Rettangolo dati grigio
			Rettangolo(Pagine[i].riquadro[j].Height-height_title,Pagine[i].riquadro[j].Width,Pagine[i].riquadro[j].Top+height_title,Pagine[i].riquadro[j].Left,1);
			for( int k = 0;k<Pagine[i].riquadro[j].lunghezza;k++) // for every box
			{
				//cout<<"\nPrima dell'IF : "<<Pagine[i].riquadro[j].nomi[k];
				if(Pagine[i].riquadro[j].Tipo == "DI")
				{
				int Next_Text_L = 0;
				int Next_Text_T = 0;
				int Next_Circle_L = 0;
				int Next_Circle_T = 0;
				
				Next_Text_T = Pagine[i].riquadro[j].Top+Y_t*(k+2)-15;
				Next_Text_L = Pagine[i].riquadro[j].Left+Z+Space;
				
				Next_Circle_T = Next_Text_T;
				Next_Circle_L = Pagine[i].riquadro[j].Left+10;
				
				//CERCHIO
				Cerchio(Z,Next_Circle_T,Pagine[i].riquadro[j].nomi[k],Next_Circle_L,Z);

				//Testo
				Testo(Pagine[i].riquadro[j].nomi[k],Y_t,Pagine[i].riquadro[j].maxlen,Next_Text_T,Next_Text_L);
				
				//cout<<"\n"<<i<<" : "<<Pagine[i].riquadro[j].nomi[k];
				}
				if(Pagine[i].riquadro[j].Tipo == "AI")
				{
				int Next_IO_L = 0;
				int Next_IO_T = 0;
				int Next_Text_L = 0;
				int Next_Text_T = 0;
				
				Next_Text_T = Titolo_Top+Y_t*(k+1);
				Next_Text_L = Titolo_Left;				
				
				Next_IO_T = Next_Text_T;
				Next_IO_L = Next_Text_L+Pagine[i].riquadro[j].maxlen+Space;
			
				//IO_field
				IO_field(30,Next_IO_T,Pagine[i].riquadro[j].nomi[k],Next_IO_L,70,0,0);
				
				//Testo
				Testo(Pagine[i].riquadro[j].nomi[k],Y_t,Pagine[i].riquadro[j].maxlen,Next_Text_T,Next_Text_L);
				//cout<<"\n"<<i<<" : "<<Pagine[i].riquadro[j].nomi[k];
				}
				if(Pagine[i].riquadro[j].Tipo == "DQ")
				{
				
				int Next_Text_L = 0;
				int Next_Text_T = 0;
				int Next_Button_set_T = 0;
				int Next_Button_set_L = 0;
				int Next_Button_Reset_T = 0;
				int Next_Button_Reset_L = 0;
				
				Next_Text_T = Titolo_Top+Y_t*(k+1);
				Next_Text_L = Titolo_Left;
				
				Next_Button_set_T = Next_Text_T;
				Next_Button_set_L = Next_Text_L+Pagine[i].riquadro[j].maxlen+Space;
				int width_set = 40;
				int height_set = 25;
				
				
				Next_Button_Reset_T = Next_Text_T;
				Next_Button_Reset_L = Next_Button_set_L+10+width_set;
				int width_reset = 60;
				int height_reset = 25;
				
				
				//Testo
				Testo(Pagine[i].riquadro[j].nomi[k],Y_t,Pagine[i].riquadro[j].maxlen,Next_Text_T,Next_Text_L);
				
				//Set
				Pulsante(height_set,width_set,Next_Button_set_T,Next_Button_set_L,Pagine[i].riquadro[j].nomi[k],1);
				
				//Reset
				Pulsante(height_reset,width_reset,Next_Button_Reset_T,Next_Button_Reset_L,Pagine[i].riquadro[j].nomi[k],0);
				//cout<<"\n"<<j<<" : "<<Pagine[i].riquadro[j].nomi[k];
				}
				if(Pagine[i].riquadro[j].Tipo == "AQ")
				{
				int Next_IO_L = 0;
				int Next_IO_T = 0;
				int Next_Text_L = 0;
				int Next_Text_T = 0;
				
				Next_Text_T = Titolo_Top+Y_t*(k+1);
				Next_Text_L = Titolo_Left;				
				
				Next_IO_T = Next_Text_T;
				Next_IO_L = Next_Text_L+Pagine[i].riquadro[j].maxlen+Space;

				//IO_field
				IO_field(30,Next_IO_T,Pagine[i].riquadro[j].nomi[k],Next_IO_L,70,1,0);
				
				//Testo
				Testo(Pagine[i].riquadro[j].nomi[k],Y_t,Pagine[i].riquadro[j].maxlen,Next_Text_T,Next_Text_L);
				
				//cout<<"\n"<<i<<" : "<<Pagine[i].riquadro[j].nomi[k];
				}
				cout<<"\n"<<i<<" : "<<Pagine[i].riquadro[j].nomi[k];
			}
			Fine_Gruppo();
			
		}
		
	// 1 next
	// 2 prev
	// 3 home

	// Next
	Pulsante_pagine(height_buttons,width_buttons,Next_T, Next_L, next_page,1);
	// Prev
	Pulsante_pagine(height_buttons,width_buttons,Prev_T, Prev_L, prev_page,2);
	// Home
	Pulsante_pagine(height_buttons,width_buttons,Home_T, Home_L, "Home",3);
	
	Chiusura();
	}
	
}


int main(){
	
	cout<<"Il file csv deve avere le seguenti colonne : TIPO_DATI,NOMI_COMPLETI,TAG,PAROLA_CHIAVE,CHIUSURA\nLa colonna di CHIUSURA e'composta da soli ';'\n ";
	Start();
	Conta_elementi();
	cout<<"Numero di elementi presenti nel file excel : "<<n_elements<<endl;
	
	string nomi_completi[n_elements];
	string tag[n_elements];
	string parole_chiave[n_elements];
	string tipo_dati[n_elements];
	
	Estrai_colonna(numero_colonna_excel,tipo_dati);
	Estrai_colonna(numero_colonna_excel+1,nomi_completi);
	Estrai_colonna(numero_colonna_excel+2,tag);
	Estrai_colonna(numero_colonna_excel+3,parole_chiave);
	
	Riempi_cassetti(tag,nomi_completi,tipo_dati);
	for(int i = 0;i<n_cassetti;i++)
	cout<<"\ntipo : "<<box[i].Tipo;
	/*for(int i = 0;i<n_cassetti;i++)
	for(int j = 0;j<box[i].lunghezza;j++)
	cout<<"\nNomi completi : "<<box[i].nomi[j] << " i = "<<i<< " /"<<n_cassetti;
	
	cout<<"\nNomi completi check : "<<box[n_cassetti-1].nomi[0] ;*/
	Inizializzazione();
	
	Parametriche();
	cout<<"\nFinito";
	string fine;
	cin>>fine;
}
