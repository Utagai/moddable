/*
 * Copyright (c) 2025-2026  Moddable Tech, Inc.
 *
 *   This file is part of the Moddable SDK Runtime.
 * 
 *   The Moddable SDK Runtime is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   The Moddable SDK Runtime is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with the Moddable SDK Runtime.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

class Message extends Native("xs_appmessage_destructor") {
	constructor(options) {
		super();
		const keys = options?.keys;
		if (Array.isArray(keys)) {
			options = {
				...options,
				keys: new Map(keys.map((value, index) => [value, 10000 + index]))
			}
		}
		native("xs_appmessage").call(this, options);
	};
	close() { return native("xs_appmessage_close").call(this); };

	read() {
		const {keys, map} = native("xs_appmessage_read").call(this);
		trace(`[message.read] keys map size=${keys?.size ?? 0}\n`);
		if (keys) {
			for (const [key, code] of keys.entries()) {
				trace(`[message.read] checking key=${JSON.stringify(key)} code=${code} has=${map.has(code)} val=${map.has(code) ? JSON.stringify(map.get(code)) : "N/A"}\n`);
				if (map.has(code)) {
					map.set(key, map.get(code));
					map.delete(code);
				}
			}
		}
		trace(`[message.read] final map size=${map.size} keys=[${Array.from(map.keys()).map(k => JSON.stringify(k)).join(", ")}]\n`);
		if (map.has(15007)) {
			trace(`[message.read] has 15007; value: ${map.get(15007)}\n`);
		}
		return map;
	}
	write(map) {
		native("xs_appmessage_write").call(this, map, Array.from(map.keys()));
	}

	match(code, keys) {		// private to implementation... maybe just pass to constructor
		for (const value of keys.values()) {
			if (value === code)
				return true;
		}
	}

	get format() {
		return "map";
	}
	set format(value) {
		if ("map" != value)
				throw new RangeError("only map");
	}
	get input() {
 		return native("xs_appmessage_get_input").call(this);
	}
	get output() {
 		return native("xs_appmessage_get_output").call(this);		
	}
}

export default Message;
