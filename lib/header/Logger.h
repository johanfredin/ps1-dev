#ifndef PSX_LOGGER_H
#define PSX_LOGGER_H

#define OFF 0
#define ERROR 1
#define WARN 2
#define INFO 3
#define DEBUG 4
#define TRACE 5

#define LOG_LEVEL INFO
#define ENABLED (LOG_LEVEL > OFF)

/**
 * Log a message at specified level
 * @param level log level (0=off, 1=error, 2=warn, 3=info, 4=debug, 5=trace)
 * @param src_file the name of the C source/header file that called the log function (will be included in log message)
 * @param src_func the name of the function that called the log function (will be included in log message)
 * @param msg the message to print
 */
void logr_log(unsigned char level, char *src_file, char *src_func, char *msg, ...);

#define LOGR_LOG_GOBJ(level, gobj)                                                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*********************");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*   Game Object     *");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*********************");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "w=%d", (gobj)->w);             \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "h=%d", (gobj)->h);             \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "image=%d", (gobj)->sprite);   \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "x_speed=%d", (gobj)->x_speed); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "y_speed=%d", (gobj)->y_speed); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "health=%d", (gobj)->health);   \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "spawn_x=%d", (gobj)->spawn_x); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "spawn_y=%d", (gobj)->spawn_y); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "type=%d", (gobj)->type)

#define LOGR_LOG_TELEPORT(level, teleport)                                              \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*********************");          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*   Teleport        *");          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*********************");          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "x=%d", (&(teleport))->origin.x);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "y=%d", (&(teleport))->origin.y);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "w=%d", (&(teleport))->origin.w);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "h=%d", (&(teleport))->origin.h);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_x=%d", (&(teleport))->dest_x); \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_y=%d", (&(teleport))->dest_y); \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_frame=%d", (&(teleport))->dest_frame)

#define LOGR_LOG_SPRITE(level, sprite) \
    logr_log(level, "Logger.h", "LOGR_LOG_SPRITE", "SPRITE ADDED: {x:%d, y:%d, w:%d, h:%d, u:%d, v:%d}", (sprite)->x, (sprite)->y, (sprite)->w, (sprite)->h, (sprite)->u, (sprite)->v)

#define LOGR_LOG_GS_OBJ(level, gs_obj) logr_log(level, "Logger.h", "LOGR_LOG_GS_OBJ", "image x, y, u, v = {%d, %d, %d, %d}", (gs_obj)->x, (gs_obj)->y, (gs_obj)->u, (gs_obj)->v)

#define LOGR_LOG_DLG(level, dlg) \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                           \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*   Dialog          *");                           \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                           \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "id=%s", (dlg)->id);                                \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "x=%d", (dlg)->x);                                  \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "y=%d", (dlg)->y);                                  \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "visible=%d", (dlg)->visible);                      \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "ticks_per_frame=%d", (dlg)->ticks_per_frame);      \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "n_messages=%d", (dlg)->n_messages)


#define LOGR_LOG_MSG(level, msg) \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*   Message         *");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "strlen=%d", (msg)->strlen);                \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "acc_chars=%d", (msg)->acc_chars);          \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "active=%d", (msg)->active);                \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "acc_ticks=%d", (msg)->acc_ticks)

#define LOGR_LOG_GS_CELL(level, cell) \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "*********************");                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "*   GsCEL           *");                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "*********************");                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "u=%d", (cell)->u);                          \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "v=%d", (cell)->v);                          \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "cba=%d", (cell)->cba);                      \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "flag=%d", (cell)->flag);                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_CEL", "tpage=%d", (cell)->tpage)

#define LOGR_LOG_GS_MAP(level, map) \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "*   GsMAP           *");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "ncellw=%d", (map)->ncellw);                \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "ncellh=%d", (map)->ncellh);                \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "cellw=%d", (map)->cellw);                  \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "cellh=%d", (map)->cellh);                  \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "base=%p", (map)->base);                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_MAP", "index=%p", (map)->index)

#define LOGR_LOG_GS_BG(level, bg)  \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "*   GsBG            *");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "attribute=%d", (bg)->attribute);           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "x=%d", (bg)->x);                           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "y=%d", (bg)->y);                           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "scrollx=%d", (bg)->scrollx);               \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "r=%d", (bg)->r);                           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "g=%d", (bg)->g);                           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "b=%d", (bg)->b);                           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "map=%p", (bg)->map);                       \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "mx=%d", (bg)->mx);                         \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "my=%d", (bg)->my);                         \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "scalex=%d", (bg)->scalex);                 \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "scaley=%d", (bg)->scaley);                 \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "rotate=%d", (bg)->rotate);                 \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "w=%d", (bg)->w);                           \
    logr_log(level, "Logger.h", "LOGR_LOG_GS_BG", "h=%d", (bg)->h)

#define LOGR_LOG_FRAME_BUFFER(level, rect)                                                    \
    logr_log(level, "Logger.h", "LOGR_LOG_FRAME_BUFFER", "Framebuffer coords assigned at {x:%d, y:%d, w:%d, h:%d}", (rect)->x, (rect)->y, (rect)->w, (rect)->h)

#define LOGR_LOG_CLUT(level, rect)                                                    \
    logr_log(level, "Logger.h", "LOGR_LOG_FRAME_CLUT", "CLUT coords assigned at {x:%d, y:%d, w:%d, h:%d}", (rect)->x, (rect)->y, (rect)->w, (rect)->h)

#endif // PSX_LOGGER_H
