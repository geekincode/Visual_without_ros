# Project Site: Visual_without_ros

This is the source for the project site for [Visual_without_ros](https://github.com/geekincode/Visual_without_ros).

> **Note**: This project site is hosted using GitHub Pages, with the `dist/` folder committed to the `gh-pages` branch of this repository. See [the "GitHub Pages" section](#github-pages) below for more information.

## Development

This is a simple Vite + React + TypeScript application. To run it locally:

```bash
npm install
npm run dev
```

Then visit `http://localhost:5173` (or similar).

To build the static assets, run:

```bash
npm run build
```

The resulting build will be in the `dist/` folder.

### GitHub Pages

This project uses the [`gh-pages`](https://www.npmjs.com/package/gh-pages) package to deploy the site to GitHub Pages.

The workflow is as follows:

1. Build the site (`npm run build`)
2. Commit the resulting `dist/` folder to the `gh-pages` branch
3. Push the `gh-pages` branch to GitHub

This is all handled by running:

```bash
npm run deploy
```

> **Note**: The `dist/` folder is committed to the `gh-pages` branch, not the `main` branch. This is intentional, as it allows GitHub Pages to serve the built site directly from that branch.