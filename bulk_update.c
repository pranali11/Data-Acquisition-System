/* the file is just for just for reference which will help to understand to upload the document into the server. */

#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>
	#include <json-c/json.h>
	#include <curl/curl.h>

json_object *json;
json_object *json_2;
json_object *jarray;

	struct curl_fetch_st {
	    char *payload;
	    size_t size;
	};
	Static int callback(void *data, int argc, char **argv, char **azColName)
	{
     	int i;
     	json = json_object_new_object();
	fprintf(stdderr, "database data= %s\n", (const char*)data);
        for(i= 0, i<argc; i++)
	{
		printf("%s =%s\n",azColName[i],argv[i] ? argv[i]: "NULL")
	}
		printf("\n");
		if (dataflag==1)
		{
			json_object_object_add(json,"created_at",json_object_new_string(argv[1]));
			json_object_object_add(json,"field1",json_object_new_string(argv[2]));
			json_object_object_add(json,"field2",json_object_new_string(argv[3]));
			json_object_object_add(json,"field3",json_object_new_string(argv[4]));
			json_object_object_add(json,"field4",json_object_new_string(argv[5]));
			json_object_object_add(json,"field5",json_object_new_string(argv[6]));
			json_object_object_add(json,"field6",json_object_new_string(argv[7]));
			json_object_object_add(json,"field7",json_object_new_string(argv[8]));
			json_object_array_add(jarray,json)

		}

	}

	/* callback for curl fetch */
	size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
	    size_t realsize = size * nmemb;                             /* calculate buffer size */
	    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */
	

	    /* expand buffer */
	    p->payload = (char *) realloc(p->payload, p->size + realsize + 1);
	

	    /* check buffer */
	    if (p->payload == NULL) {
	      /* this isn't good */
	      fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
	      /* free buffer */
	      free(p->payload);
	      /* return */
	      return -1;
	    }
	
	    /* copy contents to buffer */
	    memcpy(&(p->payload[p->size]), contents, realsize);
	
	    /* set new buffer size */
	    p->size += realsize;
	

	    /* ensure null termination */
	    p->payload[p->size] = 0;
	

	    /* return size */
	    return realsize;
	}
	

	/* fetch and return url body via curl */
	CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
	    CURLcode rcode;                   /* curl result code */
	

	    /* init payload */
	    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));
	

	    /* check payload */
	    if (fetch->payload == NULL) {
	        /* log error */
	        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
	        /* return error */
	        return CURLE_FAILED_INIT;
	    }
	

	    /* init size */
	    fetch->size = 0;
	

	    /* set url to fetch */
	    curl_easy_setopt(ch, CURLOPT_URL, url);
	

	    /* set calback function */
	    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);
	

	    /* pass fetch struct pointer */
	    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);
	

	    /* set default user agent */
	    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	

	    /* set timeout */
	    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);
	

	    /* enable location redirects */
	    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
	

	    /* set maximum allowed redirects */
	    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);
	

	    /* fetch the url */
	    rcode = curl_easy_perform(ch);
	

	    /* return */
	    return rcode;
	}
	
/*---------------------------------------------------------------------- main program ------------------------------------------------- */
	int main(int argc, char *argv[]) {
	    CURL *ch;                                               /* curl handle */
	    CURLcode rcode;                                         /* curl result code */
	

	    json_object *json;                                      /* json post body */
	    enum json_tokener_error jerr = json_tokener_success;    /* json parse error */
	

	    struct curl_fetch_st curl_fetch;                        /* curl fetch struct */
	    struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
	    struct curl_slist *headers = NULL;                      /* http headers to send with request */
	
		sqlite3 *db;
		char *zErrMsg =0;
		int rc;
		char *sql;
		const char* data= 'callback function called';
	    /* url to test site */
	    char *url = "http://api.thingspeak.com/channels/358554/bulk_update.json";
	

	    /* init curl handle */
	    if ((ch = curl_easy_init()) == NULL) {
	        /* log error */
	        fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
	        /* return error */
	        return 1;
	    }
		rc= sqlite3_open("test2.db",&db)	

		if (!rc)
		{
			fprintf(stdout,"Opened database");	
		}
	    
	
	    json_2 = json_object_new_object();
	    /* create json object for post */
	    jarray = json_object_new_array();
	
	    json_object_object_add(json_2,"write_api_key", json_object_new_string('LDVGVGTVBHBI123'));
	    sql = "SELECT * FROM Sensors LIMIT 4";
		data_flag=1;
		sqlite3_close(db);
		
	    /* build post data */
	    
		/* set content type */
	    headers = curl_slist_append(headers, "Accept: application/json");
	    headers = curl_slist_append(headers, "Content-Type: application/json");
		
		json_object_object_add(json_2,"updates", jarray);
		printf("%s\n", json_object_to_json_string_ext(json_2, JSON_C_TO_STRING_SPACED| JSON_C_TO_STRING_PRETTY));

	    /* set curl options */
	    curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "POST");
	    curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);
	    curl_easy_setopt(ch, CURLOPT_POSTFIELDS, json_object_to_json_string(json_2));
	

	    /* fetch page and capture return code */
	    rcode = curl_fetch_url(ch, url, cf);
	

	    /* cleanup curl handle */
	    curl_easy_cleanup(ch);
	

	    /* free headers */
	    curl_slist_free_all(headers);
	
	    /* free json object */
	    json_object_put(json);
	
	    /* check return code */
	    if (rcode != CURLE_OK || cf->size < 1) {
	        /* log error */
	        fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s",
	            url, curl_easy_strerror(rcode));
	        /* return error */
	        return 2;
	    }
	
	    /* check payload */
	    if (cf->payload != NULL) {
	        /* print result */
	        printf("CURL Returned: \n%s\n", cf->payload);
	        /* parse return */
	        json = json_tokener_parse_verbose(cf->payload, &jerr);
	        /* free payload */
	        free(cf->payload);
	    } else {
	        /* error */
	        fprintf(stderr, "ERROR: Failed to populate payload");
	        /* free payload */
	        free(cf->payload);
	        /* return */
	        return 3;
	    }
	

	    /* check error */
	    if (jerr != json_tokener_success) {
	        /* error */
	        fprintf(stderr, "ERROR: Failed to parse json string");
	        /* free json object */
	        json_object_put(json);
	        /* return */
	        return 4;
	    }
	
	    /* debugging */
	    printf("Parsed JSON: %s\n", json_object_to_json_string(json));
	
	    /* exit */
	    return 0;
	}
