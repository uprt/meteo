#include <getopt.h>
#include <unistd.h>
#include <iostream>
#include <utility>

#include "app.h"
#include "config.h"

void meteod::Config::parseCMDLine(int argc, char *argv[]) {
  static struct option longOpts[] = {{"help", no_argument, NULL, 'h'},
                                     {"file", required_argument, NULL, 'f'},
                                     {"parity", required_argument, NULL, 'r'},
                                     {"port", required_argument, NULL, 'p'},
                                     {"protocol", required_argument, NULL, 'i'},
                                     {"speed", required_argument, NULL, 's'},
                                     {"stopbits", required_argument, NULL, 'b'},
                                     {"timeout", required_argument, NULL, 't'},
                                     {"verbose", no_argument, NULL, 'v'},
                                     {NULL, no_argument, NULL, 0}};

  static char const *optString = "?hf:r:p:i:s:b:t:v";

  int opt = 0;
  int longIndex = 0;

  opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
  while (opt != -1) {
    switch (opt) {
      case 'b':
        if (optarg) {
          int stopbits = atoi(optarg);
          if (stopbits == 2) {
            this->double_stopbits = true;
          } else if (stopbits == 1) {
            this->double_stopbits = false;
          } else {
            std::cerr << "Incorrect stopbits value in `-b`: " << optarg
                      << std::endl;
          }
        } else {
          std::cerr << "Incorrect stopbits value in `-b`" << std::endl;
        }
        break;
      case 'f':
        if (optarg) {
          this->result_file = optarg;
        } else {
          std::cerr << "Incorrect file path in `-f`" << std::endl;
        }
        break;
      case '?':
      case 'h':
        App::printUsage();
        exit(0);
        break;
      case 'i':
        if (optarg) {
          this->proto = atoi(optarg);
        } else {
          std::cerr << "Incorrect argument value in `-i`" << std::endl;
        }
        break;
      case 'p':
        if (optarg) {
          this->port = optarg;
        } else {
          std::cerr << "Incorrect port speed value in `-p`" << std::endl;
        }
        break;
      case 'r':
        if (optarg) {
          int parity = atoi(optarg);
          if (parity > 2) {
            std::cerr << "Incorrect parity value in `-r`: " << optarg
                      << std::endl;
          } else {
            this->parity = parity;
          }
        } else {
          std::cerr << "Incorrect parity value in `-r`" << std::endl;
        }
        break;
      case 's':
        if (optarg) {
          int speed = atoi(optarg);
          if (speed == 0) {
            std::cerr << "Incorrect port speed value in `-s`: " << optarg
                      << std::endl;
          } else {
            this->speed = speed;
          }
        } else {
          std::cerr << "Incorrect argument value in `-s`" << std::endl;
        }
        break;
      case 't':
        if (optarg) {
          int parity = atoi(optarg);
          this->silence_timeout = parity;
        } else {
          std::cerr << "Incorrect silenceTimeout value in `-p`" << std::endl;
        }
        break;
      case 'v':
        this->debug_log = true;
        break;
      default:
        break;
    }
    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
  }
}
