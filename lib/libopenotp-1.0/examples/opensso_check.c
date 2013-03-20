#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opensso.h>

void usage(char *prog) {
   printf("Usage: %s <OPENSSO_URL> <SESSION> [-dt | --data <DATA>]\n", prog); 
   fflush(stdout);
   exit(1);
}

void print_response(struct opensso_check_rep_t *rep) {
   printf ("Response Code: ");
   switch (rep->code) {
    case 0: printf ("Failure\n");
      break;
    case 1: printf ("Success\n");
      break;
    default: printf("Unknown (%d)\n", rep->code);
      break;
   }
   if (rep->message) printf ("Message: %s\n", rep->message);
   if (rep->data) printf ("Data: %s\n", rep->data);
   if (rep->timeout) printf ("Timeout: %d\n", rep->timeout);
   fflush(stdout);
}

int main(int argc, char *argv[]) {
   opensso_check_rep_t *rep;
   opensso_check_req_t *req;
   int i;
   
   void _log(char *str) {
      printf("%s\n", str);
   }
   
   if (argc<3) usage(argv[0]);
   
   if (!opensso_initialize(argv[1], NULL, NULL, NULL, 0, &_log)) exit(1);
   
   req = opensso_check_req_new();
   req->session = strdup(argv[2]);
   
   for (i=3; i<argc; i+=2) {
      if (i+1==argc) usage(argv[0]);
      if (strcmp(argv[i], "-dt") == 0 || strcmp(argv[i], "--data") == 0) req->data = strdup(argv[i+1]);
      else usage(argv[0]);
   }
   
   rep = opensso_check(req, &_log);
   if (!rep) {
      printf("Invalid openssoCheck response\n");
      exit(1);
   }
   print_response(rep);
   
   opensso_check_req_free(req);
   opensso_check_rep_free(rep);
   opensso_terminate(&_log);
   exit(0);
}
