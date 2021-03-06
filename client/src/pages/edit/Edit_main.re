open Utils;
open Edit_state;

module S = {
  open Css;
  open Ds_unit;

  let header_element =
    [
      borderBottom(BorderWidth.bw1, `solid, Color.light_3),
      after([
        height(`px(3)),
        contentRule(" "),
        width(`percent(100.)),
        position(`absolute),
        left(`zero),
        unsafe("background", "linear-gradient(to right, #36C1D7, #514B9C)"),
      ]),
    ]
    ->style;

  let header =
    [
      height(`px(38)),
      paddingLeft(Space.s2),
      paddingRight(Space.s2),
      // padding2(~v=Space.s2, ~h=Space.s4),
      display(`flex),
      alignItems(`center),
      justifyContent(`spaceBetween),
    ]
    ->style;

  let sidebar = [display(`flex), flexDirection(`column)]->style;

  let preview_container =
    [position(`relative), width(`percent(100.)), height(`percent(100.))]
    ->style;

  let frame =
    [
      position(`absolute),
      top(`zero),
      right(`zero),
      bottom(`zero),
      left(`zero),
      borderStyle(`none),
      width(`percent(100.)),
      height(`percent(100.)),
    ]
    ->style;

  let overlay =
    [
      position(`absolute),
      top(`zero),
      right(`zero),
      bottom(`zero),
      left(`zero),
      borderStyle(`none),
      width(`percent(100.)),
      height(`percent(100.)),
      zIndex(1),
      fontSize(`rem(2.)),
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
      height(`percent(100.)),
    ]
    ->style;
};

let default_value_react = {code|let str = React.string;

module Counter = {
  [@react.component]
  let make = (~name) => {
    let (count, setCount) = React.useState(() => 0);

    <div>
      <p> {str({j|$name clicked $count times|j})} </p>
      <button onClick={_ => setCount(_ => count + 1)}>
        {str("Click me")}
      </button>
    </div>
  };
};

ReactDOMRe.renderToElementWithId(<Counter name="Counter" />, "root");|code};

let make_initial_files = value => {
  Belt.Map.String.empty
  ->Belt.Map.String.set("index.re", Edit_state.make_file(value))
  ->Belt.Map.String.set(
      "index.html",
      Edit_state.make_file({html|<div id="root"></div>|html}),
    )
  ->Belt.Map.String.set("style.css", Edit_state.make_file(""));
};

%bs.raw
{|require("react-reflex/styles.css")|};

[@react.component]
let make = (~value=default_value_react) => {
  let (state, send) =
    ReactUpdate.useReducer(
      {
        files: make_initial_files(value),
        active_file: "index.re",
        iframe_ref: ref(Js.Nullable.null),
        log: [||],
        evaling: false,
      },
      reducer,
    );

  React.useEffect1(
    () => {
      send(Compile_active_file);
      None;
    },
    [|
      state.files
      ->Belt.Map.String.get(state.active_file)
      ->Belt.Option.map(file => file.code),
    |],
  );

  React.useEffect0(() => {
    open Frame_comm;
    window->addMessageListener(
      event => {
        let data = event##data;
        switch (data##_type->Js.Nullable.toOption) {
        | Some("sketch") => send(Frame_msg(data##payload))
        | _ => ()
        };
      },
      false,
    );
    None;
  });

  <Reflex.Container orientation="horizontal">
    <Reflex.Element className=S.header_element flex=0.058>
      <header className=S.header>
        <div> <Ds.Logo sizeInPx=32 /> </div>
        <button onClick={_ => send(Update_url)}> "Share"->str </button>
        <div>
          <a
            href="https://github.com/Sketch-sh/nit/issues/new" target="_blank">
            "Bug report"->str
          </a>
        </div>
      </header>
    </Reflex.Element>
    <Reflex.Element>
      <Reflex.Container orientation="vertical">
        <Reflex.Element>
          {switch (state.files->Belt.Map.String.get(state.active_file)) {
           | None => "Nothing to show here"->str
           | Some({code, errors, warnings, compiled}) =>
             <Edit_editor
               value=code
               errors
               warnings
               ?compiled
               onChange={new_value =>
                 send(File_update(state.active_file, new_value))
               }
             />
           }}
        </Reflex.Element>
        <Reflex.Splitter className=Ds.splitter_className />
        <Reflex.Element>
          <Reflex.Container orientation="horizontal">
            <Reflex.Element>
              <section className=S.preview_container>
                {state.evaling
                   ? <div className=S.overlay> "Compiling..."->str </div>
                   : React.null}
                <iframe
                  id="frame"
                  className=S.frame
                  src="/container.html"
                  ref={ReactDOMRe.Ref.callbackDomRef(r =>
                    state.iframe_ref := r
                  )}
                  sandbox="allow-modals allow-scripts allow-popups allow-forms allow-same-origin"
                />
              </section>
            </Reflex.Element>
            <Reflex.Splitter className=Ds.splitter_className />
            <Reflex.Element>
              <ConsoleFeed.Display logs={state.log} styles=Ds.console_style />
            </Reflex.Element>
          </Reflex.Container>
        </Reflex.Element>
      </Reflex.Container>
    </Reflex.Element>
  </Reflex.Container>;
};
