# SVOBODA
Structured Validation Of Board Optimization Data API

Based almost entire on an early version of GULAG

depends on libmicrohttpd libjson-c

todo:
deprecate config.conf/args -> just have each setting be an API request, either at startup or within each request
deprecate weights -> again just a part of each API request
deprecate layout storage -> the API doesn't need to remember these whoever calls the API should have the foresight to remember
