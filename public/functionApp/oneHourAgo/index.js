module.exports = function (context, req) {
    context.log('JavaScript HTTP trigger function processed a request.');

    if (req.query.period || (req.body && req.body.period)) {
        context.res = {
            // status: 200, /* Defaults to 200 */
            body: {
                "period": (req.query.period || req.body.period),
                "data": context.bindings.inputDocument
            }
        };
    }
    else {
        context.res = {
            status: 400,
            body: "Please pass a parameter 'period' on the query string or in the request body"
        };
    }
    context.done();
};