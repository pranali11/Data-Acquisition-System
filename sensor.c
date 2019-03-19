//Data Acqisition System IoT data
/* There are all together 2 analog IC- 8591, one -MPC-27048(Expansion IC) these communicate with raspberry pi with I2C protocol.  */

#include <wiringPi.h>
#include <pcf8591.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sqlite3.h>
#include <json/json.h>
#include <wiringPi.h>
#include <time.h>
#include <string.h>
#include <wiringPiI2C.h>

#define Address_1 0x48 //pcf8591- address is hardcoded as 0x00
#define Address_2 0x49 //pcf8591- address is 0x01
// changing the base address of the IC for accessing the channels of pcf8591.
#define BASE_1 64
#define BASE_2 74

#define A0 BASE_1+0 
#define A1 BASE_1+1
#define A2 BASE_1+2
#define A3 BASE_1+3
#define A4 BASE_2+0
#define A5 BASE_2+1
#define A6 BASE_2+2
#define A7 BASE_2+3

//declarating the functions
void temperature_LM35();
void temperature_thermistor();
void intensity();
void voltage();
void current();

//defining the variables as gobally
float temp_lm35,temp_thermistor, intensity,vtg,current;
int ldr,AQI;

int main(void)
{
	wiringPiSetup();
	pcf8591Setup(BASE_1,Address_1)
	pcf8591Setup(BASE_2,Address_2)
	rc= sqlite3_open("test_2.db",&db);
	if(!rc)
	{
		fprintf(stdout,"Opened database");
	}
	sql= CREATE TABLE COMPANY(
  		 ID INT PRIMARY KEY     NOT NULL,
   		NAME           TEXT    NOT NULL,
   		AGE            INT     NOT NULL,
   		ADDRESS        CHAR(50),
   		SALARY         REAL
);
	while(1)
	{
		//eight analog inputs - 4 will be potentiometer
		temp_lm35= temperature_LM35();
		temp_thermistor = temperature_thermistor();
		ldr = intensity();
		vtg= voltage();
		current = current_reading();
		digitalIO();
		database();				
	}
}
/* -----------------------here the temperature sensor is LM-35 --------------------------------*/
void temperature_LM35()
{
	int temp_raw;
	float temp_vtg;
	temp_raw = analogRead(A0);
	temp_vtg = (0.2f)(temp_raw*5.0)/255;
	temp_lm35= temp_vtg/10;
	return(temp)
}
/* ----------------------- thermistor- as the temperature changes the resistance also changes ----------------------------*/
void temperature_thermistor()
{
	// thermistor calibration
	int raw;
	float vtg, ka = 0.001283, kb = 0.0002362 ,kc = 0.00000009285;
	raw= analogRead(A1)
	vtg = (	raw* 5.0)/255;
	//using voltage divider rule 
	float Rt,x,thermistor;
	Rt = (1000*vtg)/(5.0-vtg);
	x= ln(Rt);
	thermistor = 1/(ka+kb*x + kc*(x^3));
	return(thermistor);
}	
/* LDR - reading will be between 0 -255 since the pcf8591 is 8 bit ADC */

void intensity()
{
	int ldr;
	ldr = analogRead(A2);
	return(ldr)
}
/*------------------------------- potentiometer reading --------------------------------*/
void voltage()
{
	int raw
	float vtg;
	raw = analogRead(A3);
	vtg= (raw* 5.0)/255;
	return(vtg)
}

/*------------------------------------- current --------------------------------------*/
void current_reading()
{
	int RawValue = analogRead(A4);
	float volt = (RawValue / 255.0) * 5000;
	float Amps = ((volt - 2500)/ 185);
	return(Amps)
}

void AQI()
{
	int RawValue = analogRead(A5);
	int volt = (RawValue * 5)/ 255;
	AQI = volt * 100;
	return(Amps)
}

void digitalIO()
{
	fd_1 = wiringPiI2CSetup(0x20);
	sw= wiringPiI2CReadReg8(fd_1, 0x13);
	printf("switch number = %d", sw)
	swtich(sw)
	{
		case 0x07:
		printf("swtich 1 pressed\n");
		break;

		case 0x0b:
		printf("swtich 2 pressed\n");
		break;

		case 0x0d:
		printf("swtich 3 pressed\n");
		break;

		case 0x0e:
		printf("swtich 4 pressed\n");
		break;

		default :
		printf("swtich 1 pressed\n");
		break;
	}
	
}
 
void database()
{
	time(&rawtime);
	info= localtime(&rawtime);
	if (rc)
	{
	fprintf(stderr,"Can't open database: %s\n",sqlite3_errmsg(db));
	}
	else
	{
		fprintf(stdout,"opened successfully");
	}
	strftime(buf_hr,10,"%X", info)
	strftime(buf_YY,10,"%Y", info)
	strftime(buf_MM,10,"%m", info)
	strftime(buf_DD,10,"%d", info)
	
	strcat(buf_YY,"-");
	strcat(buf_YY,buf_MM);
	strcat(buf_YY,"-");
	strcat(buf_YY,buf_DD);
	
	strcpy (collect, "INSERT INTO SENSORS(date_time,temperature_lm35, temp_termistor, LDR, Current, Vtg,AQI)"\
	"VALUES(" );
	sprintf(data_time,"'%sT%sZ'", buf_YY,buf_hr)
	sprintf(data_tem_lm35,"%0.2f", temp_lm35)
	sprintf(data_temp_termistor,"%0.2f", temp_termistor)
	sprintf(data_LDR,"%d", ldr)
	sprintf(data_current,"%0.2f", current)
	sprintf(data_voltage,"%0.2f", voltage)
	sprintf(data_AQI,"%d", AQI)

	strcat(collect,data_time)		
	strcat(collect,data_tem_lm35)
	strcat(collect,data_temp_termistor)
	strcat(collect,data_LDR)
	strcat(collect,data_current)
	strcat(collect,data_voltage)
	strcat(collect,data_AQI)

	sql= collect;
	
	rc= sqlite3_exec(db, sql, callback,0,&zErrMsg)
	
	if(rc != SQLITE_OK){
	fprintf("sqlerror= %s\n", zErrMsg);
	sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	

	
}


	
