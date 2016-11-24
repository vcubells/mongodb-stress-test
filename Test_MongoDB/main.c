//
//  main.c
//  Test_MongoDB
//
//  Created by Vicente Cubells Nonell on 12/08/16.
//  Copyright © 2016 Vicente Cubells Nonell. All rights reserved.
//

/* 
 
 gcc main.c -o test-mongo-c $(pkg-config --cflags --libs libmongoc-1.0) -pthread
 
 o
 
 gcc main.c -o test-mongo-c  -I/usr/local/include -lmongoc-1.0 -lbson-1.0 -pthread
 
*/

/* ./test-mongo-c [CONNECTION_STRING] [DATABASE_NAME] [COLLECTION_NAME] */


#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

#define N_THREADS 8
#define N_INSERT 10000000

const char *database_name = "test";
const char *collection_name = "test";

static void * worker (void *data) {
    mongoc_client_pool_t *pool = data;
    mongoc_client_t      *client;
    
    mongoc_collection_t *collection;
    mongoc_database_t    *database;
    
    bson_error_t error;
    
    long int number;
    int i = 0;
    
    bson_t* doc;
    
    client = mongoc_client_pool_pop (pool);
    
    /* Do something... */
    
    database = mongoc_client_get_database (client, database_name);
    collection = mongoc_client_get_collection (client, database_name, collection_name);
    
    
    for (i = 0; i < N_INSERT; ++i) {
        
                number = rand() % N_INSERT + 1;
        
                doc = BCON_NEW ("number", BCON_INT32 (number));
        
                if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
                    fprintf (stderr, "%s\n", error.message);
                }
        
                bson_destroy (doc);
                doc = NULL;
            }
    
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    
    /* End */
    
    mongoc_client_pool_push (pool, client);
    
    pthread_exit(NULL);
}


int main(int argc, const char * argv[]) {
   
    mongoc_client_pool_t *pool;
    
    pthread_t  threads[N_THREADS];
    
    mongoc_uri_t * uri = (mongoc_uri_t * ) "mongodb://127.0.0.1:27017";
    
    int i = 0;
    
    srand(time(NULL));
    
    if (argc > 1) {
        uri = (mongoc_uri_t *) argv [1];
    }
    
    if (argc > 2) {
        database_name = argv [2];
    }
    
    if (argc > 3) {
        collection_name = argv [3];
    }

    mongoc_init ();
    
    uri = mongoc_uri_new ((const char *) uri);
    
    pool = mongoc_client_pool_new (uri);
    
    for (i = 0; i < N_THREADS; i++) {
        pthread_create (&threads[i], NULL, worker, pool);
    }
    
    for (i = 0; i < N_THREADS; i++) {
        pthread_join (threads[i], NULL);
    }
    
    mongoc_client_pool_destroy (pool);
    mongoc_uri_destroy (uri);
    mongoc_cleanup ();
    
    return EXIT_SUCCESS;
}
