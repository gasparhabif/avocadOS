#include "proceso1.h"

int main()
{
    t_cpu_conf config = get_cpu_config("../cfg/config.cfg");
    t_log *logger = log_create("./logs/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso 1! %s", config.ip_mongo);
    log_destroy(logger);
    free(config);
}