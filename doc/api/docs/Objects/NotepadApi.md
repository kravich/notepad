# NotepadApi

Provides access to the Notepad API.

[TOC]

## NotepadApi.connect()
## NotepadApi.connect(connectedCallback)
## NotepadApi.connect(socketPath, extensionId)
## NotepadApi.connect(socketPath, extensionId, connectedCallback)

Connects to Notepad and returns a new [`NotepadApi`](NotepadApi)
instance.

If a function is provided with `connectedCallback`, it will be called
as soon as the connection is completed. The parameters passed to the
callback are:

  - `api`: the constructed [`NotepadApi`](NotepadApi) object.
  
If `socketPath` and `extensionId` are provided, they'll be used for the
connection. Otherwise, their values are taken respectively from the first and
the second command line argument (with Node, they correspond to
`process.argv[2]` and `process.argv[3]`).

For example:

    var NotepadApi = require("notepad-api").NotepadApi

    NotepadApi.connect(function(api) {
        // Connected
    });

## api.onWindowInitialization(callback)

Launch a callback for each currently open window and for each future window.

This is preferable to the `'newWindow'` event of Notepad, because it could
happen that the extension isn't ready soon enough to receive the `'newWindow'`
event for the first window. This method, instead, ensures that the passed
callback will be called once and only once for each current or future window.

Example:

    var NotepadApi = require("notepad-api").NotepadApi

    NotepadApi.connect(function(api) {
        api.onWindowInitialization(function(window) {
            // Do something
        });
    });

## api.extensionId

Get the id assigned to this extension by Notepad.

## api.notepad

Get an instance of the main [`Notepad`](Notepad) object.