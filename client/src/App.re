%raw
{|require("./index.css");|};

let parse_query_string: (string, string) => string = [%raw
  {|
  function(name, query) {
    console.log(name, query);
    name = name.replace(/[\[]/, '\\[').replace(/[\]]/, '\\]');
    var regex = new RegExp('[\\?&]' + name + '=([^&#]*)');
    var results = regex.exec(query);
    return results === null ? '' : decodeURIComponent(results[1].replace(/\+/g, ' '));
}
|}
];

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let query = url.ReasonReactRouter.search;

  let value =
    switch (parse_query_string("code", query)) {
    | "" => None
    | compressed_string => LzString.URI.decompress(compressed_string)
    };

  <Edit_main ?value />;
};

let default = make;
