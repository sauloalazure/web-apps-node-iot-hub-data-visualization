module.exports = function (context, IoTHubMessages) {
    context.log(`JavaScript eventhub trigger function called for message array ${IoTHubMessages}`);
    
    var messages = [];

    IoTHubMessages.forEach(message => {
        context.log(`Processed message`, message);
        // messages.push( JSON.stringify(message) );
        context.bindings.outputDocument = JSON.stringify(message);
    });

    context.done();
};