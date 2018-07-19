// Generated by BUCKLESCRIPT VERSION 4.0.0, PLEASE EDIT WITH CARE

import * as ReasonReact from "reason-react/src/ReasonReact.js";
import * as Js_primitive from "bs-platform/lib/es6/js_primitive.js";
import * as ReactTextareaAutosize from "react-textarea-autosize";

function make(className, value, onChange, onKeyDown, autoFocus, minRows, maxRows, children) {
  var tmp = {
    value: value
  };
  if (className) {
    tmp.className = Js_primitive.valFromOption(className);
  }
  if (onChange) {
    tmp.onChange = Js_primitive.valFromOption(onChange);
  }
  if (onKeyDown) {
    tmp.onKeyDown = Js_primitive.valFromOption(onKeyDown);
  }
  if (autoFocus) {
    tmp.autoFocus = Js_primitive.valFromOption(autoFocus);
  }
  if (minRows) {
    tmp.minRows = Js_primitive.valFromOption(minRows);
  }
  if (maxRows) {
    tmp.maxRows = Js_primitive.valFromOption(maxRows);
  }
  return ReasonReact.wrapJsForReason(ReactTextareaAutosize.default, tmp, children);
}

export {
  make ,
  
}
/* ReasonReact Not a pure module */
