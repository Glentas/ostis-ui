/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <sc-memory/sc_keynodes.hpp>

#include "AbstractUIModel.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

namespace htmlTranslationModule
{
class HTMLTagMapper
{
public:
  static std::unordered_map<ScKeynode::HashType, UIComponentType> const & GetUIComponentTypeByKeynodeHash()
  {
    static auto const UIComponentTypeByKeynodeHash = std::unordered_map<ScKeynode::HashType, UIComponentType>(
        {{HTMLTranslatorKeynodes::concept_ui_image.Hash(), UIComponentType::Image},
         {HTMLTranslatorKeynodes::concept_ui_sound.Hash(), UIComponentType::Sound},
         {HTMLTranslatorKeynodes::concept_ui_text.Hash(), UIComponentType::Text},
         {HTMLTranslatorKeynodes::concept_ui_divider.Hash(), UIComponentType::Divider},
         {HTMLTranslatorKeynodes::concept_ui_spacer.Hash(), UIComponentType::Spacer},
         {HTMLTranslatorKeynodes::concept_ui_menu.Hash(), UIComponentType::Menu},
         {HTMLTranslatorKeynodes::concept_ui_menu_bar.Hash(), UIComponentType::MenuBar},
         {HTMLTranslatorKeynodes::concept_ui_list.Hash(), UIComponentType::List},
         {HTMLTranslatorKeynodes::concept_ui_scroll.Hash(), UIComponentType::Scroll},
         {HTMLTranslatorKeynodes::concept_ui_window.Hash(), UIComponentType::Window},
         {HTMLTranslatorKeynodes::concept_ui_text_field.Hash(), UIComponentType::TextField},
         {HTMLTranslatorKeynodes::concept_ui_text_area.Hash(), UIComponentType::TextArea},
         {HTMLTranslatorKeynodes::concept_ui_slider.Hash(), UIComponentType::Slider},
         {HTMLTranslatorKeynodes::concept_ui_select.Hash(), UIComponentType::Select},
         {HTMLTranslatorKeynodes::concept_ui_button.Hash(), UIComponentType::Button},
         {HTMLTranslatorKeynodes::concept_ui_menu_item.Hash(), UIComponentType::MenuItem}});
    return UIComponentTypeByKeynodeHash;
  }

  static std::unordered_map<ScKeynode::HashType, UIParamType> const & GetUIParamTypeByKeynodeHash()
  {
    static auto const UIParamTypeByKeynodeHash = std::unordered_map<ScKeynode::HashType, UIParamType>(
        {{HTMLTranslatorKeynodes::concept_ui_name.Hash(), UIParamType::Name},
         {HTMLTranslatorKeynodes::concept_ui_x.Hash(), UIParamType::X},
         {HTMLTranslatorKeynodes::concept_ui_y.Hash(), UIParamType::Y},
         {HTMLTranslatorKeynodes::concept_ui_width.Hash(), UIParamType::Width},
         {HTMLTranslatorKeynodes::concept_ui_height.Hash(), UIParamType::Height},
         {HTMLTranslatorKeynodes::concept_ui_order.Hash(), UIParamType::Order},
         {HTMLTranslatorKeynodes::concept_ui_visible.Hash(), UIParamType::Visible},
         {HTMLTranslatorKeynodes::concept_ui_enabled.Hash(), UIParamType::Enabled},
         {HTMLTranslatorKeynodes::concept_ui_opacity.Hash(), UIParamType::Opacity},
         {HTMLTranslatorKeynodes::concept_ui_text_value.Hash(), UIParamType::TextValue},
         {HTMLTranslatorKeynodes::concept_ui_tooltip.Hash(), UIParamType::Tooltip},
         {HTMLTranslatorKeynodes::concept_ui_action_id.Hash(), UIParamType::ActionId},
         {HTMLTranslatorKeynodes::concept_ui_background_color.Hash(), UIParamType::BackgroundColor},
         {HTMLTranslatorKeynodes::concept_ui_color.Hash(), UIParamType::Color},
         {HTMLTranslatorKeynodes::concept_ui_border_color.Hash(), UIParamType::BorderColor},
         {HTMLTranslatorKeynodes::concept_ui_border_width.Hash(), UIParamType::BorderWidth},
         {HTMLTranslatorKeynodes::concept_ui_border_style.Hash(), UIParamType::BorderStyle},
         {HTMLTranslatorKeynodes::concept_ui_corner_radius.Hash(), UIParamType::CornerRadius},
         {HTMLTranslatorKeynodes::concept_ui_font_size.Hash(), UIParamType::FontSize},
         {HTMLTranslatorKeynodes::concept_ui_font_weight.Hash(), UIParamType::FontWeight},
         {HTMLTranslatorKeynodes::concept_ui_text_align.Hash(), UIParamType::TextAlign},
         {HTMLTranslatorKeynodes::concept_ui_padding.Hash(), UIParamType::Padding},
         {HTMLTranslatorKeynodes::concept_ui_margin.Hash(), UIParamType::Margin},
         {HTMLTranslatorKeynodes::concept_ui_min_width.Hash(), UIParamType::MinWidth},
         {HTMLTranslatorKeynodes::concept_ui_min_height.Hash(), UIParamType::MinHeight},
         {HTMLTranslatorKeynodes::concept_ui_max_width.Hash(), UIParamType::MaxWidth},
         {HTMLTranslatorKeynodes::concept_ui_max_height.Hash(), UIParamType::MaxHeight}});
    return UIParamTypeByKeynodeHash;
  };

  static std::unordered_map<ScKeynode::HashType, UILinkType> const & GetUILinkTypeByKeynodeClass()
  {
    static auto const UILinkTypeByKeynode = std::unordered_map<ScKeynode::HashType, UILinkType>(
        {{HTMLTranslatorKeynodes::nrel_ui_contains.Hash(), UILinkType::Contains}});
    return UILinkTypeByKeynode;
  };

  static inline std::unordered_map<UIComponentType, std::string_view> const HTMLTemplateByUIComponentType = {
      {UIComponentType::Image, "<img id=\"{}\"style=\"position: absolute;\n{}\" src=\"{}\" alt=\"\">"},
      {UIComponentType::Sound, "<audio id=\"{}\" style=\"position: absolute;\n{}\" src=\"{}\" controls></audio>"},
      {UIComponentType::Text, "<p id=\"{}\" style=\"position: absolute; font-family: monospace;\n{}\">{}</p>"},
      {UIComponentType::Divider, "<hr id=\"{}\" style=\"position: absolute;\n{}\">"},
      {UIComponentType::Spacer, "<div id=\"{}\" style=\"position: absolute;\n{}\">{}</div>"},
      {UIComponentType::Menu, "<nav id=\"{}\" style=\"position: absolute;\n{}\">{}</nav>"},
      {UIComponentType::MenuBar, "<div id=\"{}\" role=\"menubar\" style=\"position: absolute;\n{}\">{}</div>"},
      {UIComponentType::List, "<ul id=\"{}\" style=\"position: absolute;\n{}\">{}</ul>"},
      {UIComponentType::Scroll,
       "<div id=\"{}\" style=\"position: absolute; overflow-y: auto; overflow-x: hidden; box-sizing: "
       "border-box;\n{}\">{}</div>"},
      {UIComponentType::Window, "<section id=\"{}\" role=\"dialog\" style=\"position: absolute;\n{}\">{}</section>"},
      {UIComponentType::TextField,
       "<input id=\"{}\" type=\"text\" style=\"position: absolute; box-sizing: border-box;\n{}\" value=\"{}\">"},
      {UIComponentType::TextArea,
       "<textarea id=\"{}\" style=\"position: absolute; box-sizing: border-box;\n{}\">{}</textarea>"},
      {UIComponentType::Slider,
       "<input id=\"{}\" type=\"range\" style=\"position: absolute; box-sizing: border-box;\n{}\" value=\"{}\">"},
      {UIComponentType::Select,
       "<select id=\"{}\" style=\"position: absolute; cursor: pointer; box-sizing: border-box;\n{}\">{}</select>"},
      {UIComponentType::Button,
       "<button type=\"button\" id=\"{}\" onclick=\"var b=this.style.backgroundColor; this.style.background='#444444'; "
       "setTimeout(() "
       "=> this.style.backgroundColor=b, 200)\" style=\"position: absolute; cursor: pointer;\n{}\">{}</button>"},
      {UIComponentType::MenuItem,
       "<li id=\"{}\" role=\"menuitem\" style=\"position: absolute; list-style: none;\n{}\">{}</li>"}};

  static inline std::unordered_map<UIParamType, std::string_view> const HTMLTemplateByUIParamType = {
      {UIParamType::X, R"(left: {};)"},
      {UIParamType::Y, R"(top: {};)"},
      {UIParamType::Width, R"(width: {};)"},
      {UIParamType::Height, R"(height: {};)"},
      {UIParamType::Order, R"(z-index: {};)"},
      {UIParamType::Visible, R"(visibility: {};)"},
      {UIParamType::Opacity, R"(opacity: {};)"},
      {UIParamType::BackgroundColor, R"(background-color: {};)"},
      {UIParamType::Color, R"(color: {};)"},
      {UIParamType::BorderColor, R"(border-color: {};)"},
      {UIParamType::BorderWidth, R"(border-width: {};)"},
      {UIParamType::BorderStyle, R"(border-style: {};)"},
      {UIParamType::CornerRadius, R"(border-radius: {};)"},
      {UIParamType::FontSize, R"(font-size: {};)"},
      {UIParamType::FontWeight, R"(font-weight: {};)"},
      {UIParamType::TextAlign, R"(text-align: {};)"},
      {UIParamType::Padding, R"(padding: {};)"},
      {UIParamType::Margin, R"(margin: {};)"},
      {UIParamType::MinWidth, R"(min-width: {};)"},
      {UIParamType::MinHeight, R"(min-height: {};)"},
      {UIParamType::MaxWidth, R"(max-width: {};)"},
      {UIParamType::MaxHeight, R"(max-height: {};)"}};

  static inline std::string GetFullHTML(std::string const & bodyContent)
  {
    std::string script = R"(
var E = {
    exports: {}
};
var O = E.exports,
    P;

function M() {
    return P || (P = 1, (function(L, B) {
        (function(N, T) {
            L.exports = T()
        })(O, (() => (() => {
            var N = [function(k, s, o) {
                    var t = this && this.__createBinding || (Object.create ? function(l, i, y, d) {
                            d === void 0 && (d = y);
                            var u = Object.getOwnPropertyDescriptor(i, y);
                            u && !("get" in u ? !i.__esModule : u.writable || u.configurable) || (u = {
                                enumerable: !0,
                                get: function() {
                                    return i[y]
                                }
                            }), Object.defineProperty(l, d, u)
                        } : function(l, i, y, d) {
                            d === void 0 && (d = y), l[d] = i[y]
                        }),
                        e = this && this.__exportStar || function(l, i) {
                            for (var y in l) y === "default" || Object.prototype.hasOwnProperty.call(i, y) || t(i, l, y)
                        };
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), e(o(1), s), e(o(2), s), e(o(10), s), e(o(11), s), e(o(4), s), e(o(12), s), e(o(5), s), e(o(13), s), e(o(14), s), e(o(6), s), e(o(7), s), e(o(15), s), e(o(8), s), e(o(3), s)
                }, (k, s) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScAddr = void 0, s.ScAddr = class {
                        constructor(o = 0) {
                            this._value = o
                        }
                        get value() {
                            return this._value
                        }
                        isValid() {
                            return this._value != 0
                        }
                        equal(o) {
                            return this._value === o._value
                        }
                    }
                }, function(k, s, o) {
                    var t = this && this.__awaiter || function(n, c, a, _) {
                        return new(a || (a = Promise))((function(r, p) {
                            function v(C) {
                                try {
                                    g(_.next(C))
                                } catch (S) {
                                    p(S)
                                }
                            }

                            function f(C) {
                                try {
                                    g(_.throw(C))
                                } catch (S) {
                                    p(S)
                                }
                            }

                            function g(C) {
                                var S;
                                C.done ? r(C.value) : (S = C.value, S instanceof a ? S : new a((function(A) {
                                    A(S)
                                }))).then(v, f)
                            }
                            g((_ = _.apply(n, c || [])).next())
                        }))
                    };
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScClient = void 0;
                    const e = o(3),
                        l = o(1),
                        i = o(4),
                        y = o(5),
                        d = o(6),
                        u = o(7),
                        m = o(9),
                        w = o(8),
                        h = {
                            keynodesCacheSize: w.DEFAULT_KEYNODES_CACHE_SIZE
                        };
                    s.ScClient = class {
                        constructor(n, c = h) {
                            var a;
                            this.sendMessagesFromQueue = () => {
                                this._messageQueue.forEach((_ => _())), this._messageQueue = []
                            }, this.onMessage = _ => {
                                var r;
                                const p = JSON.parse(_.data.toString()),
                                    v = p.id,
                                    f = this._callbacks[v];
                                if (p.event) {
                                    const g = this._events[v];
                                    if (!g) throw `Can't find callback for an event ${v}`;
                                    (r = g.callback) === null || r === void 0 || r.call(g, new l.ScAddr(p.payload[0]), new l.ScAddr(p.payload[1]), new l.ScAddr(p.payload[2]), g.id)
                                } else {
                                    if (!f) throw `Can't find callback for a command ${v}`;
                                    delete this._callbacks[v], f(p)
                                }
                            }, this._socket = typeof n == "string" ? new WebSocket(n) : n, this._socket.onmessage = this.onMessage, this._socket.onopen = this.sendMessagesFromQueue, this._messageQueue = [], this._callbacks = {}, this._events = {}, this._eventID = 1, this._keynodesCacheSize = (a = c.keynodesCacheSize) !== null && a !== void 0 ? a : w.DEFAULT_KEYNODES_CACHE_SIZE, this._keynodesCache = new Map
                        }
                        addEventListener(n, c) {
                            this._socket.addEventListener(n, c)
                        }
                        removeEventListener(n, c) {
                            this._socket.removeEventListener(n, c)
                        }
                        sendMessage(n, c, a) {
                            if (this._eventID++, this._callbacks[this._eventID]) throw "Invalid state of messages queue";
                            this._callbacks[this._eventID] = a;
                            const _ = JSON.stringify({
                                    id: this._eventID,
                                    type: n,
                                    payload: c
                                }),
                                r = () => this._socket.send(_);
                            this._socket.readyState === this._socket.OPEN ? r() : this._messageQueue.push(r)
                        }
                        resolveOrReject(n, c, a, _) {
                            return _.length === 0 ? n(a) : c(typeof _ == "string" ? _ : _.map((({
                                message: r
                            }) => r)))
                        }
                        getUser() {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((n, c) => {
                                    this.sendMessage("connection_info", null, (({
                                        payload: a,
                                        errors: _
                                    }) => {
                                        const r = new l.ScAddr(a.user_addr);
                                        this.resolveOrReject(n, c, r, _)
                                    }))
                                }))
                            }))
                        }
                        getElementsTypes(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    if (!n.length) return c([]);
                                    const _ = n.map((({
                                        value: r
                                    }) => r));
                                    this.sendMessage("check_elements", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        const v = r.map((f => new u.ScType(f)));
                                        this.resolveOrReject(c, a, v, p)
                                    }))
                                }))
                            }))
                        }
                        checkElements(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `checkElements` method is deprecated. Use `getElementsTypes` instead."), this.getElementsTypes(n)
                            }))
                        }
                        generateElements(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.commands.map((r => r.type.isLink() ? {
                                        el: "link",
                                        type: r.type.value,
                                        content: r.data.content,
                                        content_type: r.data.type
                                    } : r.type.isNode() ? {
                                        el: "node",
                                        type: r.type.value
                                    } : r.type.isConnector() ? {
                                        el: "edge",
                                        type: r.type.value,
                                        src: (0, m.transformConnectorInfo)(n, r.data.src),
                                        trg: (0, m.transformConnectorInfo)(n, r.data.trg)
                                    } : void(0, e.invalidValue)("Unknown type"))).filter((r => !!r));
                                    this.sendMessage("create_elements", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        const v = r.map((f => new l.ScAddr(f)));
                                        this.resolveOrReject(c, a, v, p)
                                    }))
                                }))
                            }))
                        }
                        createElements(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `createElements` method is deprecated. Use `generateElements` instead."), this.generateElements(n)
                            }))
                        }
                        generateElementsBySCs(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((r => {
                                        var p;
                                        return typeof r == "string" ? {
                                            scs: r,
                                            output_structure: 0
                                        } : {
                                            scs: r.scs,
                                            output_structure: (p = r.output_structure) === null || p === void 0 ? void 0 : p.value
                                        }
                                    }));
                                    this.sendMessage("create_elements_by_scs", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        this.resolveOrReject(c, a, r, p)
                                    }))
                                }))
                            }))
                        }
                        createElementsBySCs(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `createElementsBySCs` method is deprecated. Use `generateElementsBySCs` instead."), this.generateElementsBySCs(n)
                            }))
                        }
                        eraseElements(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((({
                                        value: r
                                    }) => r));
                                    this.sendMessage("delete_elements", _, (({
                                        status: r,
                                        errors: p
                                    }) => {
                                        this.resolveOrReject(c, a, r, p)
                                    }))
                                }))
                            }))
                        }
                        deleteElements(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `deleteElements` method is deprecated. Use `eraseElements` instead."), this.eraseElements(n)
                            }))
                        }
                        setLinkContents(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((r => {
                                        var p;
                                        return {
                                            command: "set",
                                            type: r.typeToStr(),
                                            data: r.data,
                                            addr: (p = r.addr) === null || p === void 0 ? void 0 : p.value
                                        }
                                    }));
                                    this.sendMessage("content", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        this.resolveOrReject(c, a, r, p)
                                    }))
                                }))
                            }))
                        }
                        getLinkContents(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((({
                                        value: r
                                    }) => ({
                                        command: "get",
                                        addr: r
                                    })));
                                    this.sendMessage("content", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        const v = r.map((f => new y.ScLinkContent(f.value, y.ScLinkContent.stringToType(f.type))));
                                        this.resolveOrReject(c, a, v, p)
                                    }))
                                }))
                            }))
                        }
                        searchLinksByContents(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((r => ({
                                        command: "find",
                                        data: r
                                    })));
                                    this.sendMessage("content", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        const v = r.map((f => f.map((g => new l.ScAddr(g)))));
                                        this.resolveOrReject(c, a, v, p)
                                    }))
                                }))
                            }))
                        }
                        getLinksByContents(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `getLinksByContents` method is deprecated. Use `searchLinksByContents` instead."), this.searchLinksByContents(n)
                            }))
                        }
                        searchLinksByContentSubstrings(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((r => ({
                                        command: "find_links_by_substr",
                                        data: r
                                    })));
                                    this.sendMessage("content", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        const v = r.map((f => f.map((g => new l.ScAddr(g)))));
                                        this.resolveOrReject(c, a, v, p)
                                    }))
                                }))
                            }))
                        }
                        getLinksByContentSubstrings(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `getLinksByContentSubstrings` method is deprecated. Use `searchLinksByContentSubstrings` instead."), this.searchLinksByContentSubstrings(n)
                            }))
                        }
                        searchLinkContentsByContentSubstrings(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((r => ({
                                        command: "find_strings_by_substr",
                                        data: r
                                    })));
                                    this.sendMessage("content", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        this.resolveOrReject(c, a, r, p)
                                    }))
                                }))
                            }))
                        }
                        getLinksContentsByContentSubstrings(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `getLinksContentsByContentSubstrings` method is deprecated. Use `searchLinkContentsByContentSubstrings` instead."), this.searchLinkContentsByContentSubstrings(n)
                            }))
                        }
                        resolveKeynodes(n) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((c, a) => {
                                    const _ = n.map((({
                                        id: r,
                                        type: p
                                    }) => p.isValid() ? {
                                        command: "resolve",
                                        idtf: r,
                                        elType: p.value
                                    } : {
                                        command: "find",
                                        idtf: r
                                    }));
                                    this.sendMessage("keynodes", _, (({
                                        payload: r,
                                        errors: p
                                    }) => {
                                        const v = r.map((f => new l.ScAddr(f))).reduce(((f, g, C) => Object.assign(Object.assign({}, f), {
                                            [n[C].id]: g
                                        })), {});
                                        this.resolveOrReject(c, a, v, p)
                                    }))
                                }))
                            }))
                        }
                        processTripleItem({
                            value: n,
                            alias: c
                        }) {
                            const a = c ? {
                                alias: c
                            } : {};
                            return n instanceof l.ScAddr ? Object.assign({
                                type: "addr",
                                value: n.value
                            }, a) : n instanceof u.ScType ? Object.assign({
                                type: "type",
                                value: n.value
                            }, a) : Object.assign({
                                type: "alias",
                                value: n
                            }, a)
                        }
                        processTemplate(n) {
                            return n instanceof l.ScAddr ? {
                                type: "addr",
                                value: n.value
                            } : typeof n == "string" && /^[a-z0-9_]+$/.test(n) ? {
                                type: "idtf",
                                value: n
                            } : typeof n == "string" ? n : n.triples.map((({
                                source: c,
                                connector: a,
                                target: _
                            }) => [this.processTripleItem(c), this.processTripleItem(a), this.processTripleItem(_)]))
                        }
                        processTemplateParams(n) {
                            return Object.keys(n).reduce(((c, a) => {
                                const _ = n[a];
                                return c[a] = typeof _ == "string" ? _ : _.value, c
                            }), {})
                        }
                        searchByTemplate(n, c = {}) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((a, _) => t(this, void 0, void 0, (function*() {
                                    const r = {
                                        templ: this.processTemplate(n),
                                        params: this.processTemplateParams(c)
                                    };
                                    this.sendMessage("search_template", r, (({
                                        payload: p,
                                        status: v,
                                        errors: f
                                    }) => {
                                        if (!v) return a([]);
                                        const g = p.addrs.map((C => {
                                            const S = C.map((A => new l.ScAddr(A)));
                                            return new d.ScTemplateResult(p.aliases, S)
                                        }));
                                        this.resolveOrReject(a, _, g, f)
                                    }))
                                }))))
                            }))
                        }
                        templateSearch(n, c = {}) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `templateSearch` method is deprecated. Use `searchByTemplate` instead."), this.searchByTemplate(n, c)
                            }))
                        }
                        generateByTemplate(n, c = {}) {
                            return t(this, void 0, void 0, (function*() {
                                return new Promise(((a, _) => t(this, void 0, void 0, (function*() {
                                    const r = {
                                        templ: this.processTemplate(n),
                                        params: this.processTemplateParams(c)
                                    };
                                    this.sendMessage("generate_template", r, (({
                                        status: p,
                                        payload: v,
                                        errors: f
                                    }) => {
                                        p || a(null);
                                        const g = v.addrs.map((S => new l.ScAddr(S))),
                                            C = new d.ScTemplateResult(v.aliases, g);
                                        this.resolveOrReject(a, _, C, f)
                                    }))
                                }))))
                            }))
                        }
                        templateGenerate(n, c = {}) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `templateGenerate` method is deprecated. Use `generateByTemplate` instead."), this.generateByTemplate(n, c)
                            }))
                        }
                        createElementaryEventSubscriptions(n) {
                            return t(this, void 0, void 0, (function*() {
                                const c = Array.isArray(n) ? n : [n];
                                return new Promise(((a, _) => {
                                    const r = {
                                        create: c.map((({
                                            type: p,
                                            addr: v
                                        }) => ({
                                            type: p,
                                            addr: v.value
                                        })))
                                    };
                                    this.sendMessage("events", r, (({
                                        payload: p,
                                        errors: v
                                    }) => {
                                        const f = c.map((({
                                            callback: g,
                                            type: C
                                        }, S) => {
                                            const A = p[S],
                                                b = new i.ScEventSubscription(A, C, g);
                                            return this._events[A] = b, b
                                        }));
                                        this.resolveOrReject(a, _, f, v)
                                    }))
                                }))
                            }))
                        }
                        eventsCreate(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `eventsCreate` method is deprecated. Use `createElementaryEventSubscriptions` instead."), this.createElementaryEventSubscriptions(n)
                            }))
                        }
                        destroyElementaryEventSubscriptions(n) {
                            return t(this, void 0, void 0, (function*() {
                                const c = Array.isArray(n) ? n : [n];
                                return new Promise(((a, _) => {
                                    const r = {
                                        delete: c
                                    };
                                    this.sendMessage("events", r, (({
                                        status: p,
                                        errors: v
                                    }) => {
                                        c.forEach((f => {
                                            delete this._events[f]
                                        })), this.resolveOrReject(a, _, p, v)
                                    }))
                                }))
                            }))
                        }
                        eventsDestroy(n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `eventsDestroy` method is deprecated. Use `destroyElementaryEventSubscriptions` instead."), this.destroyElementaryEventSubscriptions(n)
                            }))
                        }
                        searchKeynodes(...n) {
                            return t(this, void 0, void 0, (function*() {
                                const c = n.filter((g => !this._keynodesCache.get(g))).map((g => ({
                                        id: g,
                                        type: u.ScType.ConstNode
                                    }))),
                                    a = n.filter((g => this._keynodesCache.get(g))),
                                    _ = this._keynodesCache.size + c.length - this._keynodesCacheSize;
                                _ > 0 && (0, m.shiftMap)(this._keynodesCache, _);
                                const r = c.length ? yield this.resolveKeynodes(c): [], p = Object.entries(r), v = a.map((g => [g, this._keynodesCache.get(g)]));
                                p.forEach((([g, C]) => this._keynodesCache.set(g, C)));
                                const f = [...p, ...v].map((([g, C]) => [(0, m.snakeToCamelCase)(g), C]));
                                return Object.fromEntries(f)
                            }))
                        }
                        findKeynodes(...n) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScClient `findKeynodes` method is deprecated. Use `searchKeynodes` instead."), this.searchKeynodes(...n)
                            }))
                        }
                    }
                }, (k, s) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.invalidValue = void 0, s.invalidValue = o => {
                        ((t, e) => {
                            throw `Invalid value: : ${e}`
                        })(0, o)
                    }
                }, (k, s) => {
                    var o;
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScEventSubscription = s.ScEventType = void 0, (o = s.ScEventType || (s.ScEventType = {})).Unknown = "unknown", o.AfterGenerateConnector = "sc_event_after_generate_connector", o.AfterGenerateOutgoingArc = "sc_event_after_generate_outgoing_arc", o.AfterGenerateIncomingArc = "sc_event_after_generate_incoming_arc", o.AfterGenerateEdge = "sc_event_after_generate_edge", o.BeforeEraseConnector = "sc_event_before_erase_connector", o.BeforeEraseOutgoingArc = "sc_event_before_erase_outgoing_arc", o.BeforeEraseIncomingArc = "sc_event_before_erase_incoming_arc", o.BeforeEraseEdge = "sc_event_before_erase_edge", o.BeforeEraseElement = "sc_event_before_erase_element", o.BeforeChangeLinkContent = "sc_event_before_change_link_content", s.ScEventSubscription = class {
                        constructor(t, e, l) {
                            this._id = 0, this._type = null, this._callback = null, this._id = t, this._type = e, this._callback = l
                        }
                        get id() {
                            return this._id
                        }
                        get type() {
                            return this._type
                        }
                        get callback() {
                            return this._callback
                        }
                        IsValid() {
                            return this._id > 0
                        }
                    }
                }, (k, s) => {
                    var o;
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScLinkContent = s.ScLinkContentType = void 0, (function(t) {
                        t[t.Int = 0] = "Int", t[t.Float = 1] = "Float", t[t.String = 2] = "String", t[t.Binary = 3] = "Binary"
                    })(o = s.ScLinkContentType || (s.ScLinkContentType = {})), s.ScLinkContent = class {
                        constructor(t, e, l) {
                            this._data = t, this._type = e, this._addr = l
                        }
                        get data() {
                            return this._data
                        }
                        get type() {
                            return this._type
                        }
                        get addr() {
                            return this._addr
                        }
                        typeToStr() {
                            switch (this._type) {
                                case o.Binary:
                                    return "binary";
                                case o.Float:
                                    return "float";
                                case o.Int:
                                    return "int";
                                default:
                                    return "string"
                            }
                        }
                        static stringToType(t) {
                            switch (t) {
                                case "binary":
                                    return o.Binary;
                                case "float":
                                    return o.Float;
                                case "int":
                                    return o.Int;
                                default:
                                    return o.String
                            }
                        }
                    }
                }, (k, s) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScTemplateResult = void 0, s.ScTemplateResult = class {
                        constructor(o, t) {
                            this._addrs = [], this._indecies = {}, this._indecies = o, this._addrs = t
                        }
                        get size() {
                            return this._addrs.length
                        }
                        get(o) {
                            return typeof o == "string" ? this._addrs[this._indecies[o]] : this._addrs[o]
                        }
                        forEachTriple(o) {
                            for (let t = 0; t < this.size; t += 3) o(this._addrs[t], this._addrs[t + 1], this._addrs[t + 2])
                        }
                    }
                }, (k, s, o) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScType = void 0;
                    const t = o(8);
                    class e {
                        constructor(i) {
                            this._value = typeof i == "number" ? i || 0 : i instanceof e && i._value || 0
                        }
                        get value() {
                            return this._value
                        }
                        hasConstancy() {
                            return (this._value & t.sc_type_constancy_mask) != 0
                        }
                        hasSubtype(i) {
                            return (this._value & i) == i
                        }
                        isConst() {
                            return this.hasSubtype(t.sc_type_const)
                        }
                        isVar() {
                            return this.hasSubtype(t.sc_type_var)
                        }
                        hasDirection() {
                            return this.isArc()
                        }
                        isNode() {
                            return this.hasSubtype(t.sc_type_node)
                        }
                        isLink() {
                            return this.hasSubtype(t.sc_type_node_link)
                        }
                        isConnector() {
                            return this.hasSubtype(t.sc_type_connector)
                        }
                        isEdge() {
                            return console.warn("Warning: ScType `isEdge` method is deprecated. Use `isConnector` instead."), this.isConnector()
                        }
                        isCommonEdge() {
                            return this.hasSubtype(t.sc_type_common_edge)
                        }
                        isArc() {
                            return this.hasSubtype(t.sc_type_arc)
                        }
                        isCommonArc() {
                            return this.hasSubtype(t.sc_type_common_arc)
                        }
                        isMembershipArc() {
                            return this.hasSubtype(t.sc_type_membership_arc)
                        }
                        isPos() {
                            return this.hasSubtype(t.sc_type_pos_arc)
                        }
                        isNeg() {
                            return this.hasSubtype(t.sc_type_neg_arc)
                        }
                        isFuz() {
                            return this.hasSubtype(t.sc_type_fuz_arc)
                        }
                        isPerm() {
                            return this.hasSubtype(t.sc_type_perm_arc)
                        }
                        isTemp() {
                            return this.hasSubtype(t.sc_type_temp_arc)
                        }
                        isActual() {
                            return this.hasSubtype(t.sc_type_actual_arc)
                        }
                        isInactual() {
                            return this.hasSubtype(t.sc_type_inactual_arc)
                        }
                        isTuple() {
                            return this.hasSubtype(t.sc_type_node_tuple)
                        }
                        isStructure() {
                            return this.hasSubtype(t.sc_type_node_structure)
                        }
                        isStruct() {
                            return console.warn("Warning: ScType `isStruct` method is deprecated. Use `isStructure` instead."), this.isStructure()
                        }
                        isRole() {
                            return this.hasSubtype(t.sc_type_node_role)
                        }
                        isNonRole() {
                            return this.hasSubtype(t.sc_type_node_non_role)
                        }
                        isClass() {
                            return this.hasSubtype(t.sc_type_node_class)
                        }
                        isSuperclass() {
                            return this.hasSubtype(t.sc_type_node_superclass)
                        }
                        isMaterial() {
                            return this.hasSubtype(t.sc_type_node_material)
                        }
                        isValid() {
                            return this._value !== 0
                        }
                        equal(i) {
                            return this._value === i._value
                        }
                        static isNotCompatibleByMask(i, y, d) {
                            const u = i & d,
                                m = y & d;
                            return u != t.sc_type_unknown && u != m
                        }
                        isExtendableTo(i) {
                            let y = this.value,
                                d = i.value;
                            if (e.isNotCompatibleByMask(y, d, t.sc_type_element_mask) || e.isNotCompatibleByMask(y, d, t.sc_type_constancy_mask)) return !1;
                            if (this.isLink()) {
                                if (!i.isLink() || (new e(y & ~t.sc_type_node_link), i = new e(d & ~t.sc_type_node_link), e.isNotCompatibleByMask(y, d, t.sc_type_node_link_mask))) return !1
                            } else if (this.isNode()) {
                                if (!i.isNode() || (new e(y & ~t.sc_type_node), i = new e(d & ~t.sc_type_node), e.isNotCompatibleByMask(y, d, t.sc_type_node_mask))) return !1
                            } else if (this.isConnector()) {
                                if (i.isConnector()) return !1;
                                if (e.isNotCompatibleByMask(y, d, t.sc_type_connector_mask)) {
                                    if (this.isCommonEdge()) {
                                        if (!i.isCommonEdge()) return !1
                                    } else if (this.isArc()) {
                                        if (!i.isArc()) return !1;
                                        if (this.isCommonArc()) {
                                            if (!i.isCommonArc()) return !1
                                        } else if (!this.isMembershipArc() && !i.isMembershipArc()) return !1
                                    }
                                }
                                if (new e(y & ~t.sc_type_connector_mask), i = new e(d & ~t.sc_type_connector_mask), e.isNotCompatibleByMask(y, d, t.sc_type_actuality_mask) || e.isNotCompatibleByMask(y, d, t.sc_type_permanency_mask) || e.isNotCompatibleByMask(y, d, t.sc_type_positivity_mask) || e.isNotCompatibleByMask(y, d, t.sc_type_fuz_arc)) return !1
                            }
                            return !0
                        }
                        merge(i) {
                            if (!this.isExtendableTo(i)) throw "Type `" + this + "` can not be extended to `" + i + "`";
                            return new e(this._value | i._value)
                        }
                        changeConst(i) {
                            const y = this._value & ~t.sc_type_constancy_mask;
                            return new e(y | (i ? t.sc_type_const : t.sc_type_var))
                        }
                    }
                    s.ScType = e, e.Unknown = new e(t.sc_type_unknown), e.Node = new e(t.sc_type_node), e.Connector = new e(t.sc_type_connector), e.CommonEdge = new e(t.sc_type_common_edge), e.Arc = new e(t.sc_type_arc), e.CommonArc = new e(t.sc_type_common_arc), e.MembershipArc = new e(t.sc_type_membership_arc), e.Const = new e(t.sc_type_const), e.Var = new e(t.sc_type_var), e.ConstNode = new e(t.sc_type_const | t.sc_type_node), e.VarNode = new e(t.sc_type_var | t.sc_type_node), e.ConstConnector = new e(t.sc_type_const | t.sc_type_connector), e.VarConnector = new e(t.sc_type_var | t.sc_type_connector), e.ConstCommonEdge = new e(t.sc_type_const | t.sc_type_common_edge), e.VarCommonEdge = new e(t.sc_type_var | t.sc_type_common_edge), e.ConstArc = new e(t.sc_type_const | t.sc_type_arc), e.VarArc = new e(t.sc_type_var | t.sc_type_arc), e.ConstCommonArc = new e(t.sc_type_const | t.sc_type_common_arc), e.VarCommonArc = new e(t.sc_type_var | t.sc_type_common_arc), e.ConstMembershipArc = new e(t.sc_type_const | t.sc_type_membership_arc), e.VarMembershipArc = new e(t.sc_type_var | t.sc_type_membership_arc), e.PermArc = new e(t.sc_type_perm_arc), e.TempArc = new e(t.sc_type_temp_arc), e.ConstPermArc = new e(t.sc_type_const | t.sc_type_perm_arc), e.VarPermArc = new e(t.sc_type_var | t.sc_type_perm_arc), e.ConstTempArc = new e(t.sc_type_const | t.sc_type_temp_arc), e.VarTempArc = new e(t.sc_type_var | t.sc_type_temp_arc), e.ActualTempArc = new e(t.sc_type_actual_arc | t.sc_type_temp_arc), e.InactualTempArc = new e(t.sc_type_inactual_arc | t.sc_type_temp_arc), e.ConstActualTempArc = new e(t.sc_type_const | t.sc_type_actual_arc | t.sc_type_temp_arc), e.VarActualTempArc = new e(t.sc_type_var | t.sc_type_actual_arc | t.sc_type_temp_arc), e.ConstInactualTempArc = new e(t.sc_type_const | t.sc_type_inactual_arc | t.sc_type_temp_arc), e.VarInactualTempArc = new e(t.sc_type_var | t.sc_type_inactual_arc | t.sc_type_temp_arc), e.PosArc = new e(t.sc_type_pos_arc), e.NegArc = new e(t.sc_type_neg_arc), e.ConstPosArc = new e(t.sc_type_const | t.sc_type_pos_arc), e.VarPosArc = new e(t.sc_type_var | t.sc_type_pos_arc), e.PermPosArc = new e(t.sc_type_perm_arc | t.sc_type_pos_arc), e.TempPosArc = new e(t.sc_type_temp_arc | t.sc_type_pos_arc), e.ActualTempPosArc = new e(t.sc_type_actual_arc | t.sc_type_temp_arc | t.sc_type_pos_arc), e.InactualTempPosArc = new e(t.sc_type_inactual_arc | t.sc_type_temp_arc | t.sc_type_pos_arc), e.ConstPermPosArc = new e(t.sc_type_const | t.sc_type_perm_arc | t.sc_type_pos_arc), e.ConstTempPosArc = new e(t.sc_type_const | t.sc_type_temp_arc | t.sc_type_pos_arc), e.ConstActualTempPosArc = new e(t.sc_type_const | t.sc_type_actual_arc | t.sc_type_temp_arc | t.sc_type_pos_arc), e.ConstInactualTempPosArc = new e(t.sc_type_const | t.sc_type_inactual_arc | t.sc_type_temp_arc | t.sc_type_pos_arc), e.VarPermPosArc = new e(t.sc_type_var | t.sc_type_perm_arc | t.sc_type_pos_arc), e.VarTempPosArc = new e(t.sc_type_var | t.sc_type_temp_arc | t.sc_type_pos_arc), e.VarActualTempPosArc = new e(t.sc_type_var | t.sc_type_actual_arc | t.sc_type_temp_arc | t.sc_type_pos_arc), e.VarInactualTempPosArc = new e(t.sc_type_var | t.sc_type_inactual_arc | t.sc_type_temp_arc | t.sc_type_pos_arc), e.ConstNegArc = new e(t.sc_type_const | t.sc_type_neg_arc), e.VarNegArc = new e(t.sc_type_var | t.sc_type_neg_arc), e.PermNegArc = new e(t.sc_type_perm_arc | t.sc_type_neg_arc), e.TempNegArc = new e(t.sc_type_temp_arc | t.sc_type_neg_arc), e.ActualTempNegArc = new e(t.sc_type_actual_arc | t.sc_type_temp_arc | t.sc_type_neg_arc), e.InactualTempNegArc = new e(t.sc_type_inactual_arc | t.sc_type_temp_arc | t.sc_type_neg_arc), e.ConstPermNegArc = new e(t.sc_type_const | t.sc_type_perm_arc | t.sc_type_neg_arc), e.ConstTempNegArc = new e(t.sc_type_const | t.sc_type_temp_arc | t.sc_type_neg_arc), e.ConstActualTempNegArc = new e(t.sc_type_const | t.sc_type_actual_arc | t.sc_type_temp_arc | t.sc_type_neg_arc), e.ConstInactualTempNegArc = new e(t.sc_type_const | t.sc_type_inactual_arc | t.sc_type_temp_arc | t.sc_type_neg_arc), e.VarPermNegArc = new e(t.sc_type_var | t.sc_type_perm_arc | t.sc_type_neg_arc), e.VarTempNegArc = new e(t.sc_type_var | t.sc_type_temp_arc | t.sc_type_neg_arc), e.VarActualTempNegArc = new e(t.sc_type_var | t.sc_type_actual_arc | t.sc_type_temp_arc | t.sc_type_neg_arc), e.VarInactualTempNegArc = new e(t.sc_type_var | t.sc_type_inactual_arc | t.sc_type_temp_arc | t.sc_type_neg_arc), e.FuzArc = new e(t.sc_type_fuz_arc), e.ConstFuzArc = new e(t.sc_type_const | t.sc_type_fuz_arc), e.VarFuzArc = new e(t.sc_type_var | t.sc_type_fuz_arc), e.NodeLink = new e(t.sc_type_node_link), e.NodeLinkClass = new e(t.sc_type_node_link | t.sc_type_node_class), e.NodeTuple = new e(t.sc_type_node_tuple), e.NodeStructure = new e(t.sc_type_node_structure), e.NodeRole = new e(t.sc_type_node_role), e.NodeNonRole = new e(t.sc_type_node_non_role), e.NodeClass = new e(t.sc_type_node_class), e.NodeSuperclass = new e(t.sc_type_node_superclass), e.NodeMaterial = new e(t.sc_type_node_material), e.ConstNodeLink = new e(t.sc_type_const | t.sc_type_node_link), e.ConstNodeLinkClass = new e(t.sc_type_const | t.sc_type_node_link | t.sc_type_node_class), e.ConstNodeTuple = new e(t.sc_type_const | t.sc_type_node_tuple), e.ConstNodeStructure = new e(t.sc_type_const | t.sc_type_node_structure), e.ConstNodeRole = new e(t.sc_type_const | t.sc_type_node_role), e.ConstNodeNonRole = new e(t.sc_type_const | t.sc_type_node_non_role), e.ConstNodeClass = new e(t.sc_type_const | t.sc_type_node_class), e.ConstNodeSuperclass = new e(t.sc_type_const | t.sc_type_node_superclass), e.ConstNodeMaterial = new e(t.sc_type_const | t.sc_type_node_material), e.VarNodeLink = new e(t.sc_type_var | t.sc_type_node_link), e.VarNodeLinkClass = new e(t.sc_type_var | t.sc_type_node_link | t.sc_type_node_class), e.VarNodeTuple = new e(t.sc_type_var | t.sc_type_node_tuple), e.VarNodeStructure = new e(t.sc_type_var | t.sc_type_node_structure), e.VarNodeRole = new e(t.sc_type_var | t.sc_type_node_role), e.VarNodeNonRole = new e(t.sc_type_var | t.sc_type_node_non_role), e.VarNodeClass = new e(t.sc_type_var | t.sc_type_node_class), e.VarNodeSuperclass = new e(t.sc_type_var | t.sc_type_node_superclass), e.VarNodeMaterial = new e(t.sc_type_var | t.sc_type_node_material), e.EdgeUCommon = new e(e.CommonEdge), e.EdgeDCommon = new e(e.CommonArc), e.EdgeUCommonConst = new e(e.ConstCommonEdge), e.EdgeDCommonConst = new e(e.ConstCommonArc), e.EdgeAccess = new e(e.MembershipArc), e.EdgeAccessConstPosPerm = new e(e.ConstPermPosArc), e.EdgeAccessConstNegPerm = new e(e.ConstPermNegArc), e.EdgeAccessConstFuzPerm = new e(e.ConstFuzArc), e.EdgeAccessConstPosTemp = new e(e.ConstTempPosArc), e.EdgeAccessConstNegTemp = new e(e.ConstTempNegArc), e.EdgeAccessConstFuzTemp = new e(e.ConstFuzArc), e.EdgeUCommonVar = new e(e.VarCommonEdge), e.EdgeDCommonVar = new e(e.VarCommonArc), e.EdgeAccessVarPosPerm = new e(e.VarPermPosArc), e.EdgeAccessVarNegPerm = new e(e.VarPermNegArc), e.EdgeAccessVarFuzPerm = new e(e.VarFuzArc), e.EdgeAccessVarPosTemp = new e(e.VarTempPosArc), e.EdgeAccessVarNegTemp = new e(e.VarTempNegArc), e.EdgeAccessVarFuzTemp = new e(e.VarFuzArc), e.NodeConst = new e(e.ConstNode), e.NodeVar = new e(e.VarNode), e.Link = new e(e.NodeLink), e.LinkClass = new e(e.NodeLinkClass), e.NodeStruct = new e(e.NodeStructure), e.LinkConst = new e(e.ConstNodeLink), e.LinkConstClass = new e(e.ConstNodeLinkClass), e.NodeConstTuple = new e(e.ConstNodeTuple), e.NodeConstStruct = new e(e.ConstNodeStructure), e.NodeConstRole = new e(e.ConstNodeRole), e.NodeConstNoRole = new e(e.ConstNodeNonRole), e.NodeConstClass = new e(e.ConstNodeClass), e.NodeConstMaterial = new e(e.ConstNodeMaterial), e.LinkVar = new e(e.VarNodeLink), e.LinkVarClass = new e(e.VarNodeLinkClass), e.NodeVarStruct = new e(e.VarNodeStructure), e.NodeVarTuple = new e(e.VarNodeTuple), e.NodeVarRole = new e(e.VarNodeRole), e.NodeVarNoRole = new e(e.VarNodeNonRole), e.NodeVarClass = new e(e.VarNodeClass), e.NodeVarMaterial = new e(e.VarNodeMaterial)
                }, (k, s) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.DEFAULT_KEYNODES_CACHE_SIZE = s.sc_type_node_link_mask = s.sc_type_node_mask = s.sc_type_common_edge_mask = s.sc_type_common_arc_mask = s.sc_type_membership_arc_mask = s.sc_type_positivity_mask = s.sc_type_permanency_mask = s.sc_type_actuality_mask = s.sc_type_constancy_mask = s.sc_type_arc_mask = s.sc_type_connector_mask = s.sc_type_element_mask = s.sc_type_node_material = s.sc_type_node_superclass = s.sc_type_node_class = s.sc_type_node_non_role = s.sc_type_node_role = s.sc_type_node_structure = s.sc_type_node_tuple = s.sc_type_node_link = s.sc_type_fuz_arc = s.sc_type_neg_arc = s.sc_type_pos_arc = s.sc_type_perm_arc = s.sc_type_temp_arc = s.sc_type_inactual_arc = s.sc_type_actual_arc = s.sc_type_var = s.sc_type_const = s.sc_type_membership_arc = s.sc_type_common_arc = s.sc_type_arc = s.sc_type_common_edge = s.sc_type_connector = s.sc_type_node = s.sc_type_unknown = void 0, s.sc_type_unknown = 0, s.sc_type_node = 1, s.sc_type_connector = 16384, s.sc_type_common_edge = 4 | s.sc_type_connector, s.sc_type_arc = 32768 | s.sc_type_connector, s.sc_type_common_arc = 8 | s.sc_type_arc, s.sc_type_membership_arc = 16 | s.sc_type_arc, s.sc_type_const = 32, s.sc_type_var = 64, s.sc_type_actual_arc = 4096 | s.sc_type_membership_arc, s.sc_type_inactual_arc = 8192 | s.sc_type_membership_arc, s.sc_type_temp_arc = 1024 | s.sc_type_membership_arc, s.sc_type_perm_arc = 2048 | s.sc_type_membership_arc, s.sc_type_pos_arc = 128 | s.sc_type_membership_arc, s.sc_type_neg_arc = 256 | s.sc_type_membership_arc, s.sc_type_fuz_arc = 512 | s.sc_type_membership_arc, s.sc_type_node_link = 2 | s.sc_type_node, s.sc_type_node_tuple = 128 | s.sc_type_node, s.sc_type_node_structure = 256 | s.sc_type_node, s.sc_type_node_role = 512 | s.sc_type_node, s.sc_type_node_non_role = 1024 | s.sc_type_node, s.sc_type_node_class = 2048 | s.sc_type_node, s.sc_type_node_superclass = 4096 | s.sc_type_node, s.sc_type_node_material = 8192 | s.sc_type_node, s.sc_type_element_mask = s.sc_type_node | s.sc_type_connector, s.sc_type_connector_mask = s.sc_type_common_edge | s.sc_type_common_arc | s.sc_type_membership_arc, s.sc_type_arc_mask = s.sc_type_common_arc | s.sc_type_membership_arc, s.sc_type_constancy_mask = s.sc_type_const | s.sc_type_var, s.sc_type_actuality_mask = s.sc_type_actual_arc | s.sc_type_inactual_arc, s.sc_type_permanency_mask = s.sc_type_perm_arc | s.sc_type_temp_arc, s.sc_type_positivity_mask = s.sc_type_pos_arc | s.sc_type_neg_arc, s.sc_type_membership_arc_mask = s.sc_type_actuality_mask | s.sc_type_permanency_mask | s.sc_type_positivity_mask | s.sc_type_fuz_arc, s.sc_type_common_arc_mask = s.sc_type_common_arc, s.sc_type_common_edge_mask = s.sc_type_common_edge, s.sc_type_node_mask = s.sc_type_node_link | s.sc_type_node_tuple | s.sc_type_node_structure | s.sc_type_node_role | s.sc_type_node_non_role | s.sc_type_node_class | s.sc_type_node_superclass | s.sc_type_node_material, s.sc_type_node_link_mask = s.sc_type_node | s.sc_type_node_link | s.sc_type_node_class, s.DEFAULT_KEYNODES_CACHE_SIZE = 5e3
                }, (k, s, o) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.snakeToCamelCase = s.shiftMap = s.transformConnectorInfo = void 0;
                    const t = o(3);
                    s.transformConnectorInfo = (e, l) => {
                        if (typeof l != "string") return {
                            type: "addr",
                            value: l.value
                        };
                        const i = e.getIndex(l);
                        return i === void 0 ? (0, t.invalidValue)(`Invalid alias: ${i}`) : {
                            type: "ref",
                            value: i
                        }
                    }, s.shiftMap = (e, l = 1) => {
                        if (l < 1) return;
                        let i = !1,
                            y = 0;
                        const d = e.keys();
                        for (; y < l && !i;) {
                            const u = d.next();
                            i = !!u.done, e.delete(u.value), y++
                        }
                    }, s.snakeToCamelCase = e => e.replace(/_(\w)/g, ((l, i) => i.toUpperCase()))
                }, (k, s, o) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScConstruction = void 0;
                    const t = o(3),
                        e = o(11);
                    s.ScConstruction = class {
                        constructor() {
                            this._commands = [], this._aliases = {}
                        }
                        generateNode(l, i) {
                            l.isNode() || (0, t.invalidValue)("You should pass node type there");
                            const y = new e.ScConstructionCommand(l);
                            i && (this._aliases[i] = this._commands.length), this._commands.push(y)
                        }
                        createNode(l, i) {
                            console.warn("Warning: ScConstruction `createNode` method is deprecated. Use `generateNode` instead."), this.generateNode(l, i)
                        }
                        generateConnector(l, i, y, d) {
                            l.isConnector() || (0, t.invalidValue)("You should pass connector type there");
                            const u = new e.ScConstructionCommand(l, {
                                src: i,
                                trg: y
                            });
                            d && (this._aliases[d] = this._commands.length), this._commands.push(u)
                        }
                        createEdge(l, i, y, d) {
                            console.warn("Warning: ScConstruction `createEdge` method is deprecated. Use `generateConnector` instead."), this.generateConnector(l, i, y, d)
                        }
                        generateLink(l, i, y) {
                            l.isLink() || (0, t.invalidValue)("You should pass link type there");
                            const d = new e.ScConstructionCommand(l, {
                                content: i.data,
                                type: i.type
                            });
                            y && (this._aliases[y] = this._commands.length), this._commands.push(d)
                        }
                        createLink(l, i, y) {
                            console.warn("Warning: ScConstruction `createLink` method is deprecated. Use `generateLink` instead."), this.generateLink(l, i, y)
                        }
                        get commands() {
                            return this._commands
                        }
                        getIndex(l) {
                            return this._aliases[l]
                        }
                    }
                }, (k, s) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScConstructionCommand = void 0, s.ScConstructionCommand = class {
                        constructor(o, t) {
                            this._elType = o, this._data = t
                        }
                        get type() {
                            return this._elType
                        }
                        get data() {
                            return this._data
                        }
                    }
                }, (k, s) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScEventSubscriptionParams = void 0, s.ScEventSubscriptionParams = class {
                        constructor(o, t, e) {
                            this._addr = o, this._type = t, this._callback = e
                        }
                        get addr() {
                            return this._addr
                        }
                        get type() {
                            return this._type
                        }
                        get callback() {
                            return this._callback
                        }
                    }
                }, function(k, s, o) {
                    var t = this && this.__awaiter || function(d, u, m, w) {
                        return new(m || (m = Promise))((function(h, n) {
                            function c(r) {
                                try {
                                    _(w.next(r))
                                } catch (p) {
                                    n(p)
                                }
                            }

                            function a(r) {
                                try {
                                    _(w.throw(r))
                                } catch (p) {
                                    n(p)
                                }
                            }

                            function _(r) {
                                var p;
                                r.done ? h(r.value) : (p = r.value, p instanceof m ? p : new m((function(v) {
                                    v(p)
                                }))).then(c, a)
                            }
                            _((w = w.apply(d, u || [])).next())
                        }))
                    };
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScSet = void 0;
                    const e = o(4),
                        l = o(12),
                        i = o(14),
                        y = o(7);
                    s.ScSet = class {
                        constructor(d, u, m, w, h, n) {
                            if (this._elements = {}, this._scClient = null, this._addr = null, this._onAdd = null, this._onRemove = null, this._onInitialize = null, this._filterType = null, this._scClient = d, this._addr = u, this._onInitialize = m, this._onAdd = w, this._onRemove = h, this._filterType = n, !this._addr || !this._addr.isValid()) throw `Invalid addr of set: ${this._addr}`
                        }
                        initialize() {
                            var d;
                            return t(this, void 0, void 0, (function*() {
                                if (!this._addr) return;
                                const u = yield(d = this._scClient) === null || d === void 0 ? void 0 : d.createElementaryEventSubscriptions([new l.ScEventSubscriptionParams(this._addr, e.ScEventType.AfterGenerateOutgoingArc, this.onEventGenerateElement.bind(this)), new l.ScEventSubscriptionParams(this._addr, e.ScEventType.BeforeEraseOutgoingArc, this.onEventEraseElement.bind(this))]);
                                return this._evtGenerateElement = u?.[0], this._evtEraseElement = u?.[1], yield this.iterateExistingElements(), new Promise((function(m) {
                                    m()
                                }))
                            }))
                        }
                        shouldAppend(d) {
                            var u;
                            return t(this, void 0, void 0, (function*() {
                                const m = yield(u = this._scClient) === null || u === void 0 ? void 0 : u.getElementsTypes(d), w = m?.map((h => !(this._filterType && (this._filterType.value & h.value) !== this._filterType.value)));
                                return new Promise((function(h) {
                                    h(w)
                                }))
                            }))
                        }
                        onEventGenerateElement(d, u, m) {
                            return t(this, void 0, void 0, (function*() {
                                if (!this._elements[u.value] && m.isValid()) {
                                    const w = yield this.shouldAppend([m]);
                                    w?.[0] && (this._elements[u.value] = m, this.callOnAdd(m))
                                }
                                return new Promise((function(w) {
                                    w()
                                }))
                            }))
                        }
                        onEventEraseElement(d, u) {
                            return t(this, void 0, void 0, (function*() {
                                const m = this._elements[u.value];
                                if (!m) throw `Invalid state of set: ${this._addr} (try to remove element ${u}, that doesn't exist)`;
                                return yield this.callOnRemove(m), delete this._elements[u.value], new Promise((function(w) {
                                    w()
                                }))
                            }))
                        }
                        callOnInitialize(d) {
                            return t(this, void 0, void 0, (function*() {
                                return this._onInitialize && (yield this._onInitialize(d)), new Promise((function(u) {
                                    u()
                                }))
                            }))
                        }
                        callOnAdd(d) {
                            return t(this, void 0, void 0, (function*() {
                                return this._onAdd && (yield this._onAdd(d)), new Promise((function(u) {
                                    u()
                                }))
                            }))
                        }
                        callOnRemove(d) {
                            return t(this, void 0, void 0, (function*() {
                                return this._onRemove && (yield this._onRemove(d)), new Promise((function(u) {
                                    u()
                                }))
                            }))
                        }
                        iterateExistingElements() {
                            return t(this, void 0, void 0, (function*() {
                                if (!this._addr || !this._scClient) return;
                                const d = [],
                                    u = new i.ScTemplate;
                                u.triple(this._addr, [y.ScType.VarPermPosArc, "_arc"], [y.ScType.Unknown, "_item"]);
                                const m = yield this._scClient.searchByTemplate(u), w = m?.map((n => n.get("_item"))) || [], h = yield this.shouldAppend(w);
                                for (let n = 0; n < m.length; ++n) {
                                    if (!h?.[n]) continue;
                                    const c = m[n].get("_arc"),
                                        a = m[n].get("_item");
                                    if (this._elements[c.value]) throw `Element ${a} already exist in set`;
                                    this._elements[c.value] = a, d.push(a)
                                }
                                return yield this.callOnInitialize(d), new Promise((function(n) {
                                    n()
                                }))
                            }))
                        }
                        addItem(d) {
                            return t(this, void 0, void 0, (function*() {
                                if (!this._addr || !this._scClient) return;
                                let u = !1;
                                const m = new i.ScTemplate;
                                if (m.triple(this._addr, [y.ScType.VarPermPosArc, "_arc"], [d, "_item"]), (yield this._scClient.searchByTemplate(m)).length == 0) {
                                    const w = yield this._scClient.generateByTemplate(m, {
                                        _item: d
                                    });
                                    if (w) {
                                        const h = w.get("_item");
                                        u = h && h.isValid()
                                    }
                                }
                                return new Promise((function(w) {
                                    w(u)
                                }))
                            }))
                        }
                    }
                }, (k, s, o) => {
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScTemplate = void 0;
                    const t = o(1),
                        e = o(7);
                    s.ScTemplate = class {
                        constructor() {
                            this._triples = []
                        }
                        get triples() {
                            return this._triples
                        }
                        triple(l, i, y) {
                            const d = this.splitTemplateParam(l),
                                u = this.splitTemplateParam(i),
                                m = this.splitTemplateParam(y);
                            return this._triples.push({
                                source: d,
                                connector: u,
                                target: m
                            }), this
                        }
                        quintuple(l, i, y, d, u) {
                            let {
                                alias: m,
                                value: w
                            } = this.splitTemplateParam(i);
                            return m || (m = `connector_1_${this._triples.length}`), this.triple(l, [w, m], y), this.triple(u, d, m), this
                        }
                        tripleWithRelation(l, i, y, d, u) {
                            return console.warn("Warning: ScTemplate `tripleWithRelation` method is deprecated. Use `quintuple` instead."), this.quintuple(l, i, y, d, u)
                        }
                        splitTemplateParam(l) {
                            if (l instanceof Array) {
                                if (l.length !== 2) throw "Invalid number of values for replacement. Use [ScType | ScAddr, string]";
                                const i = l[0],
                                    y = l[1];
                                if (!(i instanceof t.ScAddr || i instanceof e.ScType) || typeof y != "string") throw "First parameter should be ScAddr or ScType. The second one - string";
                                return {
                                    alias: y,
                                    value: i
                                }
                            }
                            return {
                                alias: null,
                                value: l
                            }
                        }
                    }
                }, function(k, s, o) {
                    var t = this && this.__awaiter || function(h, n, c, a) {
                            return new(c || (c = Promise))((function(_, r) {
                                function p(g) {
                                    try {
                                        f(a.next(g))
                                    } catch (C) {
                                        r(C)
                                    }
                                }

                                function v(g) {
                                    try {
                                        f(a.throw(g))
                                    } catch (C) {
                                        r(C)
                                    }
                                }

                                function f(g) {
                                    var C;
                                    g.done ? _(g.value) : (C = g.value, C instanceof c ? C : new c((function(S) {
                                        S(C)
                                    }))).then(p, v)
                                }
                                f((a = a.apply(h, n || [])).next())
                            }))
                        },
                        e = this && this.__rest || function(h, n) {
                            var c = {};
                            for (var a in h) Object.prototype.hasOwnProperty.call(h, a) && n.indexOf(a) < 0 && (c[a] = h[a]);
                            if (h != null && typeof Object.getOwnPropertySymbols == "function") {
                                var _ = 0;
                                for (a = Object.getOwnPropertySymbols(h); _ < a.length; _++) n.indexOf(a[_]) < 0 && Object.prototype.propertyIsEnumerable.call(h, a[_]) && (c[a[_]] = h[a[_]])
                            }
                            return c
                        };
                    Object.defineProperty(s, "__esModule", {
                        value: !0
                    }), s.ScHelper = void 0;
                    const l = o(10),
                        i = o(4),
                        y = o(12),
                        d = o(5),
                        u = o(14),
                        m = o(7),
                        w = o(9);
                    s.ScHelper = class {
                        constructor(h) {
                            this._client = h
                        }
                        getMainIdentifierLinkAddr(h, n) {
                            return t(this, void 0, void 0, (function*() {
                                const c = yield this._client.searchKeynodes("nrel_main_idtf", n), {
                                    nrelMainIdtf: a
                                } = c, _ = e(c, ["nrelMainIdtf"])[(0, w.snakeToCamelCase)(n)], r = new u.ScTemplate, p = "_link";
                                r.quintuple(h, m.ScType.VarCommonArc, [m.ScType.VarNodeLink, p], m.ScType.VarPermPosArc, a), r.triple(_, m.ScType.VarPermPosArc, p);
                                const v = yield this._client.searchByTemplate(r);
                                return v.length ? v[0].get(p) : null
                            }))
                        }
                        getMainIdentifier(h, n) {
                            return t(this, void 0, void 0, (function*() {
                                const c = yield this.getMainIdentifierLinkAddr(h, n);
                                return c ? (yield this._client.getLinkContents([c]))[0].data : null
                            }))
                        }
                        getSystemIdentifier(h) {
                            return t(this, void 0, void 0, (function*() {
                                const {
                                    nrelSystemIdentifier: n
                                } = yield this._client.searchKeynodes("nrel_system_identifier"), c = new u.ScTemplate, a = "_link";
                                c.quintuple(h, m.ScType.VarCommonArc, [m.ScType.VarNodeLink, a], m.ScType.VarPermPosArc, n);
                                const _ = yield this._client.searchByTemplate(c);
                                if (!_.length) return null;
                                const r = yield this._client.getLinkContents([_[0].get(a)]);
                                return String(r[0].data)
                            }))
                        }
                        getScIdentifier(h, n) {
                            return t(this, void 0, void 0, (function*() {
                                const c = yield this.getMainIdentifier(h, n);
                                if (c) return String(c);
                                const a = yield this.getSystemIdentifier(h);
                                return String(a || h.value)
                            }))
                        }
                        getAddrOrSystemIdentifierAddr(h) {
                            return t(this, void 0, void 0, (function*() {
                                return Number(h) || (yield this._client.searchKeynodes(String(h)))[(0, w.snakeToCamelCase)(String(h))].value
                            }))
                        }
                        getResult(h) {
                            return new Promise((n => {
                                t(this, void 0, void 0, (function*() {
                                    var c;
                                    const {
                                        nrelResult: a
                                    } = yield this._client.searchKeynodes("nrel_result"), _ = new y.ScEventSubscriptionParams(h, i.ScEventType.AfterGenerateOutgoingArc, ((g, C, S, A) => t(this, void 0, void 0, (function*() {
                                        const b = new u.ScTemplate;
                                        b.triple(a, m.ScType.VarPermPosArc, C), (yield this._client.searchByTemplate(b)).length && (this._client.destroyElementaryEventSubscriptions(A), n(S))
                                    })))), [r] = yield this._client.createElementaryEventSubscriptions(_), p = "_result", v = new u.ScTemplate;
                                    v.quintuple(h, m.ScType.VarCommonArc, [m.ScType.VarNode, p], m.ScType.VarPermPosArc, a);
                                    const f = (c = (yield this._client.searchByTemplate(v))[0]) === null || c === void 0 ? void 0 : c.get(p);
                                    f && (this._client.destroyElementaryEventSubscriptions(r.id), n(f))
                                }))
                            }))
                        }
                        getAnswer(h) {
                            return console.warn("Warning: ScHelper `getAnswer` method is deprecated. Use `getResult` instead."), this.getResult(h)
                        }
                        generateLink(h) {
                            return t(this, void 0, void 0, (function*() {
                                const n = new l.ScConstruction;
                                n.generateLink(m.ScType.ConstNodeLink, new d.ScLinkContent(h, d.ScLinkContentType.String));
                                const c = yield this._client.generateElements(n);
                                return c.length ? c[0] : null
                            }))
                        }
                        createLink(h) {
                            return t(this, void 0, void 0, (function*() {
                                return console.warn("Warning: ScHelper `createLink` method is deprecated. Use `generateLink` instead."), this.generateLink(h)
                            }))
                        }
                    }
                }],
                T = {};
            return (function k(s) {
                var o = T[s];
                if (o !== void 0) return o.exports;
                var t = T[s] = {
                    exports: {}
                };
                return N[s].call(t.exports, t, t.exports, k), t.exports
            })(0)
        })()))
    })(E)), E.exports
}
var V = M(),
    I = V.sc;
globalThis.sc = I;)";

    return std::string(
        R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8"/>
  <meta name="viewport"/>
  <title>Test interface</title>
  <style>
    body * { resize: none; }
  </style>
  <script type="module">
  )" + script
        + R"(
  </script>
</head>
<body style="margin: 2px;">
  <div style="position: relative; width: 100%; height: 100vh; container-type: size; overflow: hidden;">
  )" + bodyContent
        + R"(
  </div>
</body>
</html>
)");
  }

  static inline std::string WrapOptionComponent(
      std::string const & childID,
      std::string const & actionID,
      std::string const & conent)
  {
    return std::string("<option id=\"" + childID + "\" value=\"" + actionID + "\">" + conent + "</option>");
  }

  static inline std::string CreateSenderScript(
      std::string const & elementID,
      std::string const & actionID,
      std::string const & text)
  {
    return std::string(
        R"(<script>
      (() => {
        const btn = document.getElementById(")"
        + elementID + R"(");
        const letter = ")"
        + text + R"(";
        btn.addEventListener('click', () => {
          btn.dispatchEvent(new CustomEvent(')"
        + actionID + R"(', {
            detail: { letter },
            bubbles: true
          }));
        });
      })();
    </script>)");
  }

  static inline std::string CreateRecieverScript(std::string const & actionID)
  {
    return std::string(
        R"(<script>
  document.addEventListener(')"
        + actionID + R"(', (e) => {
    const { letter } = e.detail;
    const btn = e.target;
    alert('Content: ' + letter + '; from button: ' + btn.id + ' with id=)"
        + actionID + R"(');
  });
</script>)");
  }

  static inline std::string AgentConnectScript(std::string const & actionID, std::string const & outputID)
  {
    return std::string(
        R"(<script type="module">
const { ScClient, ScType, ScConstruction, ScLinkContent, ScLinkContentType, ScHelper } = sc;
const client = new ScClient("ws://localhost:8090/ws_json");

const kn = await client.resolveKeynodes([
  { id: ")"
        + actionID + R"(" },
  { id: "action" },
  { id: "rrel_1" },
]);
for (const [name, addr] of Object.entries(kn))
  if (!addr?.isValid()) throw `Key node is not found: ${name}`;

document.addEventListener(')"
        + actionID + R"(', async (e) => {
  const { letter } = e.detail;

  const c = new ScConstruction();
  c.generateNode(ScType.NodeConst, "act");
  c.generateConnector(ScType.EdgeAccessConstPosPerm, kn.action, "act");
  c.generateLink(ScType.LinkConst,
    new ScLinkContent(letter, ScLinkContentType.String), "letter");
  c.generateConnector(ScType.EdgeDCommonConst, "act", "letter", "arg");
  c.generateConnector(ScType.EdgeAccessConstPosPerm, kn.rrel_1, "arg");
  c.generateConnector(ScType.EdgeDCommonConst, kn[")"
        + actionID + R"("], "act");

  const [action] = await client.generateElements(c);
  const result = await new ScHelper(client).getResult(action);
  const [type] = await client.getElementsTypes([result]);
  document.getElementById(")"
        + outputID + R"(").textContent =
    type?.isLink() ? (await client.getLinkContents([result]))[0].data
                   : `ScAddr(${result.value})`;
});
</script>)");
  };
};
}  // namespace htmlTranslationModule