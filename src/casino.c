
#include <kore/kore.h>
#include <kore/http.h>

#include "assets.h"

int		page(struct http_request *);
int		page_ws_connect(struct http_request *);

void		websocket_connect(struct connection *);
void		websocket_disconnect(struct connection *);
void		websocket_message(struct connection *,
							  u_int8_t, void *, size_t);

/* Called whenever we get a new websocket connection. */
void
casino_connect(struct connection *c)
{
	kore_log(LOG_NOTICE, "%p: connected", c);
}

void
casino_message(struct connection *c, u_int8_t op, void *data, size_t len)
{
	kore_websocket_broadcast(c, op, data, len, WEBSOCKET_BROADCAST_GLOBAL);
}

void
casino_disconnect(struct connection *c)
{
	kore_log(LOG_NOTICE, "%p: disconnecting", c);
}

int
page(struct http_request *req)
{
	http_response_header(req, "content-type", "text/html");
	http_response(req, 200, asset_frontend_html, asset_len_frontend_html);

	return (KORE_RESULT_OK);
}

int
page_ws_connect(struct http_request *req)
{
	/* Perform the casino handshake, passing our callbacks. */
	kore_websocket_handshake(req, "casino_connect",
							 "casino_message", "casino_disconnect");

	return (KORE_RESULT_OK);
}
